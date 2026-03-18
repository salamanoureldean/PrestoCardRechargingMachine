#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

mkdir -p simulation_results

make clean
make

./bin/MESSAGE_QUEUE_TEST > simulation_results/message_queue_output.txt

echo "Message Queue test complete."
echo "Results in simulation_results/message_queue_output.txt"