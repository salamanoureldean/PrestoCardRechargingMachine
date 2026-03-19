CC=g++
CFLAGS=-std=c++17
INCLUDECADMIUM=-I $(CADMIUM)

bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

all: tests presto_system

build/main_money_collector_test.o: test/main_money_collector_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/main_money_collector_test.cpp -o build/main_money_collector_test.o

build/main_card_reader_test.o: test/main_card_reader_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/main_card_reader_test.cpp -o build/main_card_reader_test.o

build/main_balance_display_test.o: test/main_balance_display_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/main_balance_display_test.cpp -o build/main_balance_display_test.o

build/main_receipt_processor_test.o: test/main_receipt_processor_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/main_receipt_processor_test.cpp -o build/main_receipt_processor_test.o

build/main_message_queue_test.o: test/main_message_queue_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/main_message_queue_test.cpp -o build/main_message_queue_test.o

build/main_central_controller_test.o: test/main_central_controller_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/main_central_controller_test.cpp -o build/main_central_controller_test.o

build/main.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) top_model/main.cpp -o build/main.o

tests: build/main_money_collector_test.o build/main_card_reader_test.o build/main_balance_display_test.o build/main_receipt_processor_test.o build/main_message_queue_test.o build/main_central_controller_test.o
	$(CC) -g -o bin/MONEY_COLLECTOR_TEST build/main_money_collector_test.o
	$(CC) -g -o bin/CARD_READER_TEST build/main_card_reader_test.o
	$(CC) -g -o bin/BALANCE_DISPLAY_TEST build/main_balance_display_test.o
	$(CC) -g -o bin/RECEIPT_PROCESSOR_TEST build/main_receipt_processor_test.o
	$(CC) -g -o bin/MESSAGE_QUEUE_TEST build/main_message_queue_test.o
	$(CC) -g -o bin/CENTRAL_CONTROLLER_TEST build/main_central_controller_test.o

presto_system: build/main.o
	$(CC) -g -o bin/PRESTO_MACHINE_TEST build/main.o

clean: 
	rm -f bin/* build/*
