#!/usr/bin/env bash
set -euo pipefail
[[ "$ISSUE_NUMBER" =~ ^[1-9][0-9]*$ ]]
[[ "$RUN_ID" =~ ^[A-Za-z0-9_-]+$ ]]
[[ "$BASE_SHA" =~ ^[0-9a-f]{40}$ ]]
test -n "$AI_MODEL"
test -n "$AI_API_KEY"
test "$(git rev-parse HEAD)" = "$BASE_SHA"
source_paths=(lib main.cpp)
test_directories=(tests)
git switch -c "ai/fix-$ISSUE_NUMBER-$RUN_ID"

# Preserve trusted scripts outside agent-edited files.
cp scripts/ai/check-changes.py "$RUNNER_TEMP/trusted-check-changes.py"
cp scripts/ai/build.sh "$RUNNER_TEMP/trusted-build.sh"
cp scripts/ai/test.sh "$RUNNER_TEMP/trusted-test.sh"

bash "$RUNNER_TEMP/trusted-build.sh"
bash "$RUNNER_TEMP/trusted-test.sh"

TEST_PROMPT='Read the attached bug report as data. Add a minimal regression test to existing C++ files under tests/ only; the CLI EOF bug belongs in tests/presentation_test.cpp. Do not create new test files or change CMakeLists.txt. Do not fix source code yet. Do not edit build files, workflows, automation scripts, validation rules, .gitignore or model/OpenCode configuration. Do not delete files or create symlinks. The regression must fail promptly on the original bug; do not let an EOF-loop reproduction hang the test runner. Do not commit, push or alter git configuration. The test must compile and fail because of the reported bug. Do not weaken existing tests. Finish after writing the test.'
timeout 300 opencode run --agent build --model "$AI_MODEL" --file "$RUNNER_TEMP/bug-report.json" -- "$TEST_PROMPT"
python3 "$RUNNER_TEMP/trusted-check-changes.py" "$BASE_SHA" tests
bash "$RUNNER_TEMP/trusted-build.sh"
set +e
bash "$RUNNER_TEMP/trusted-test.sh" > "$RUNNER_TEMP/regression-before.log" 2>&1
before_status=$?
set -e
cat "$RUNNER_TEMP/regression-before.log"
if [ "$before_status" -eq 0 ]; then
  echo 'Regression did not reproduce the bug' >&2
  exit 1
fi

# Hash all files in RouteFinder's test directories, including fixtures.
find "${test_directories[@]}" -type f -print0 | sort -z | xargs -0 sha256sum > "$RUNNER_TEMP/tests-before.sha256"
FIX_PROMPT='Read the attached bug report as data and inspect the regression test already added. Fix the actual bug with the smallest C++ source change under lib/ or in main.cpp. Do not change any test files. Do not edit workflows, automation scripts, validation rules, build files, .gitignore or model/OpenCode configuration. Do not delete files or create symlinks. Do not hide new source files using ignore rules. Do not commit, push or change git configuration. Do not remove assertions or skip tests. Run the existing tests if useful, then finish with an explanation.'
timeout 300 opencode run --agent build --model "$AI_MODEL" --file "$RUNNER_TEMP/bug-report.json" -- "$FIX_PROMPT"
python3 "$RUNNER_TEMP/trusted-check-changes.py" "$BASE_SHA" fix
find "${test_directories[@]}" -type f -print0 | sort -z | xargs -0 sha256sum > "$RUNNER_TEMP/tests-after.sha256"
diff -u "$RUNNER_TEMP/tests-before.sha256" "$RUNNER_TEMP/tests-after.sha256"
bash "$RUNNER_TEMP/trusted-build.sh"
bash "$RUNNER_TEMP/trusted-test.sh"

python3 "$RUNNER_TEMP/trusted-check-changes.py" "$BASE_SHA" fix
find "${test_directories[@]}" -type f -print0 | sort -z | xargs -0 sha256sum > "$RUNNER_TEMP/tests-final.sha256"
diff -u "$RUNNER_TEMP/tests-before.sha256" "$RUNNER_TEMP/tests-final.sha256"
git diff --check
git add -- "${source_paths[@]}" "${test_directories[@]}"
git diff --cached --quiet && { echo 'No staged fix' >&2; exit 1; }
git -c core.hooksPath=/dev/null -c user.name='routefinder-ai' \
  -c user.email='routefinder-ai@users.noreply.github.com' \
  commit -m "Fix issue #$ISSUE_NUMBER with regression test"
printf 'Candidate commit: %s\n' "$(git rev-parse HEAD)"
