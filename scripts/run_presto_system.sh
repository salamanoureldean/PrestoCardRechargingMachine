#!/bin/bash
make clean
make all
./bin/PRESTO_MACHINE_TEST > simulation_results/presto_system_output.txt

echo "Full system simulation complete. Results in simulation_results/presto_system_output.txt. cat simulation_results/presto_system_output.txt to view the results."