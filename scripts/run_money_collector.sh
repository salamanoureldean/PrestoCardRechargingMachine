#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

mkdir -p simulation_results

make clean
make

./bin/MONEY_COLLECTOR_TEST > simulation_results/money_collector_output.txt

echo "Money Collector test complete."
echo "Results in simulation_results/money_collector_output.txt"