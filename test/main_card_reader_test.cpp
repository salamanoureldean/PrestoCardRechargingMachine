#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
#include <cadmium/lib/iestream.hpp>
#include <memory>
#include "../atomics/card_reader.hpp"

using namespace cadmium;
using namespace presto;

// Wrapper Coupled Model for testing
struct CardReaderTestSystem : public Coupled {
    CardReaderTestSystem(const std::string& id) : Coupled(id) {
        auto input_reader = addComponent<lib::IEStream<double>>("input_reader", "input_data/card_reader_input_test.txt");
        auto card_reader = addComponent<CardReader>("card_reader");
        
        // Couple the reader to the cardinfo_in port
        addCoupling(input_reader->out, card_reader->cardinfo_in);
    }
};

int main() {
    auto test_system = std::make_shared<CardReaderTestSystem>("cr_test_system");

    cadmium::RootCoordinator rootCoordinator(test_system);
    
    rootCoordinator.setLogger<cadmium::STDOUTLogger>(";");

    rootCoordinator.start();
    rootCoordinator.simulate(5.0);
    rootCoordinator.stop();

    return 0;
}