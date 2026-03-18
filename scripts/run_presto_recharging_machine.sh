#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

mkdir -p simulation_results

make clean
make

./bin/PRESTO_MACHINE_TEST > simulation_results/presto_system_output.txt

echo "Full system simulation complete."
echo "Results in simulation_results/presto_system_output.txt"