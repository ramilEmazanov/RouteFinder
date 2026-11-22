#!/usr/bin/env bash
set -euo pipefail

# tests/CMakeLists.txt registers labwork7_tests through gtest_discover_tests.
ctest --test-dir /tmp/routefinder-ai-build --output-on-failure --no-tests=error
