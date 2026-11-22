"""RouteFinder change guard; invoke with BASE_SHA and tests/fix phase."""
import pathlib
import subprocess
import sys

base, phase = sys.argv[1:]
if phase not in {"tests", "fix"}:
    raise SystemExit("Unknown phase")
if subprocess.check_output(["git", "rev-parse", "HEAD"], text=True).strip() != base:
    raise SystemExit("Agent changed HEAD")
tracked = subprocess.check_output(
    ["git", "diff", "--no-renames", "--name-only", "-z", base]
)
staged = subprocess.check_output(
    ["git", "diff", "--cached", "--no-renames", "--name-only", "-z", base]
)
untracked = subprocess.check_output(["git", "ls-files", "--others", "--exclude-standard", "-z"])
paths = {p.decode() for p in (tracked + staged + untracked).split(b"\0") if p}

# Global/local ignore rules must not conceal new C++ files in allowed locations.
source_suffixes = {".cpp", ".hpp"}
ignored = subprocess.check_output(
    ["git", "ls-files", "--others", "--ignored", "--exclude-standard", "-z",
     "--", "lib/", "tests/", "main.cpp"]
)
for name in ignored.split(b"\0"):
    if name and pathlib.Path(name.decode()).suffix in source_suffixes:
        raise SystemExit(f"Ignored source or test is not allowed: {name.decode()}")

if not paths:
    raise SystemExit("Agent produced no changes")
base_files = set(subprocess.check_output(
    ["git", "ls-tree", "-r", "--name-only", "-z", base]
).decode().split("\0"))
for name in paths:
    path = pathlib.Path(name)
    # Restrict extensions as well as directories: CMake, scripts and model
    # configuration remain protected even when placed under lib/ or tests/.
    is_test = name.startswith("tests/") and path.suffix in source_suffixes
    is_source = (name.startswith("lib/") and path.suffix in source_suffixes) or name == "main.cpp"
    if ".." in path.parts or not (is_test or (phase == "fix" and is_source)):
        raise SystemExit(f"Change outside allowlist: {name}")
    if phase == "tests" and name not in base_files:
        raise SystemExit(f"Regression must use an existing test file: {name}")
    if any(part.is_symlink() for part in (path, *path.parents)) or not path.is_file():
        raise SystemExit(f"Deletion or symlink is not allowed: {name}")
print(f"Validated {len(paths)} changed paths in phase {phase}")
