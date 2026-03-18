#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

mkdir -p simulation_results

make clean
make

./bin/BALANCE_DISPLAY_TEST > simulation_results/balance_display_output.txt

echo "Balance Display test complete."
echo "Results in simulation_results/balance_display_output.txt"