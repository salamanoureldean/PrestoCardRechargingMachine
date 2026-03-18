#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
#include <cadmium/lib/iestream.hpp>
#include <memory>
#include "../atomics/presto_machine.hpp"

using namespace cadmium;
using namespace presto;

struct FullSystemTest : public Coupled {
    FullSystemTest(const std::string& id) : Coupled(id) {
        // Setup input readers for each hardware port
        auto card_in = addComponent<lib::IEStream<double>>("card_in", "input_data/card_reader_input_test.txt");
        auto money_in = addComponent<lib::IEStream<double>>("money_in", "input_data/money_collector_input_test.txt");
        auto request_in = addComponent<lib::IEStream<double>>("request_in", "input_data/receipt_processor_input_test.txt");

        // Add the top level Machine
        auto presto_machine = addComponent<PrestoCardChargingMachine>("presto_machine");

        // Couple physical inputs to the machine
        addCoupling(card_in->out, presto_machine->card_in);
        addCoupling(money_in->out, presto_machine->money_in);
        addCoupling(request_in->out, presto_machine->request_in);
    }
};

int main() {
    auto test_system = std::make_shared<FullSystemTest>("full_system_test");
    cadmium::RootCoordinator rootCoordinator(test_system);
    
    rootCoordinator.setLogger<cadmium::STDOUTLogger>(";");

    rootCoordinator.start();
    rootCoordinator.simulate(50.0);
    rootCoordinator.stop();

    return 0;
}