#!/usr/bin/env bash
set -euo pipefail

# Match the system libcurl configuration in .github/workflows/cicd.yml.
# Keep generated files outside the agent's working tree.
cmake -S . -B /tmp/routefinder-ai-build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCPR_USE_SYSTEM_CURL=ON
cmake --build /tmp/routefinder-ai-build \
  --target labwork7 labwork7_tests --parallel 2
