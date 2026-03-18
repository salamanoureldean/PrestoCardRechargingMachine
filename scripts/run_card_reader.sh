#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

mkdir -p simulation_results

make clean
make

./bin/CARD_READER_TEST > simulation_results/card_reader_output.txt

echo "Card Reader test complete."
echo "Results in simulation_results/card_reader_output.txt"