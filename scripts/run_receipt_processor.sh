#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

mkdir -p simulation_results

make clean
make all

./bin/RECEIPT_PROCESSOR_TEST > simulation_results/receipt_processor_output.txt

echo "Receipt Processor test complete."
echo "CAT results in simulation_results/receipt_processor_output.txt"