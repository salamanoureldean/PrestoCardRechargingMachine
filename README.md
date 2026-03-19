PRESTO CARD CHARGING MACHINE DEVS MODEL
This folder contains the Presto Card Charging Machine DEVS model implemented in Cadmium.

REPO: https://github.com/salamanoureldean/PrestoCardRechargingMachine

FILES ORGANIZATION

- README.md      [This file] 
- PrestoCardRechargeMachine.doc 
- makefile        [Compilation instructions for tests and top model] 
 
atomics/ [This folder contains atomic model headers implemented in Cadmium] 
- balance_display.hpp
- card_reader.hpp
- central_controller.hpp
- message_queue.hpp
- money_collector.hpp
- presto_machine.hpp       [Top-level integrated coupled model] 
- receipt_processor.hpp
- recharge_controller.hpp  [Internal coupled model] 

- bin/ [This folder will be created automatically during compilation. It contains executables]

- build/ [This folder will be created automatically during compilation. It contains .o build files]

data_structures/
- message.hpp

input_data/ [This folder contains all the input event trajectories for simulation]
- balance_display_input_test.txt
- card_reader_input_test.txt
- cc_messages.txt
- message_queue_input_test.txt
- money_collector_input_test.txt
- receipt_processor_input_test.txt

scripts/ [This folder contains all the scripts that will be used to compile and build the project]
- run_presto_recharging_machine.sh
- run_money_collector.sh
- run_card_reader.sh
- run_message_queue.sh
- run_central_controller.sh
- run_receipt_processor.sh
- run_balance_display.sh

simulation_results/ [This folder stores the execution logs from simulations]
- balance_display_output.txt
- card_reader_output.txt
- central_controller_output.txt
- message_queue_output.txt
- money_collector_output.txt
- presto_system_output.txt
- receipt_processor_output.txt

test/ [This folder contains main files for atomic models] 
- main_balance_display_test.cpp
- main_card_reader_test.cpp
- main_central_controller_test.cpp
- main_message_queue_test.cpp
- main_money_collector_test.cpp
- main_receipt_processor_test.cpp

top_model/ [This folder contains the integrated Presto Machine driver] 
- main.cpp

EXECUTION STEPS

1 - Update the include path in the project 'makefile'. Ensure the following line points to your local Cadmium installation:
INCLUDECADMIUM=-I ../cadmium_v2/include

2 - Run atomic and coupled model tests: AND CAT THE RESULTING OUTPUT FILES FOR CLARITY
LINUX:
- Use the provided shell scripts to compile and run automated execution and logging of each model, each one runs make clean, make, and then executes the specified simulation:
- ./scripts/run_presto_recharging_machine.sh
- ./scripts/run_money_collector.sh
- ./scripts/run_card_reader.sh
- ./scripts/run_message_queue.sh
- ./scripts/run_central_controller.sh
- ./scripts/run_receipt_processor.sh
- ./scripts/run_balance_display.sh 

POWERSHELL:
- bash scripts/run_presto_recharging_machine.sh
- bash scripts/run_money_collector.sh
- bash scripts/run_card_reader.sh
- bash scripts/run_message_queue.sh
- bash scripts/run_central_controller.sh
- bash scripts/run_receipt_processor.sh
- bash scripts/run_balance_display.sh


 These scripts will output save text log in 'simulation_results/'..
* The .txt files in simulation_results/ are generated using the Cadmium STDOUTLogger, which includes ANSI escape codes to highlight state changes in yellow and port events in green.
* Because of these codes, the files will appear "unclean" (containing characters like [33m or [32m) if viewed in a standard text editor.
* To view the logs properly with clean, colored formatting, you must use the cat command in the terminal:
cat simulation_results/presto_system_output.txt

To execute the full system script:
- ./run_presto_recharging_machine.sh
- The system will process card insertions, money deposits, and receipt requests in FIFO order.
- To check the execution result, cat simulation_results/presto_system_output.txt in your terminal
