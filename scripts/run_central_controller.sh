#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

mkdir -p simulation_results

make clean
make

./bin/CENTRAL_CONTROLLER_TEST > simulation_results/central_controller_output.txt

echo "Central Controller test complete."
echo "Results in simulation_results/central_controller_output.txt"