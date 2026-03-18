#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
#include <cadmium/lib/iestream.hpp>
#include <memory>
#include "../atomics/money_collector.hpp"

using namespace cadmium;
using namespace presto;

struct MoneyCollectorTestSystem : public Coupled {
    MoneyCollectorTestSystem(const std::string& id) : Coupled(id) {
        auto input_reader = addComponent<lib::IEStream<double>>("input_reader", "input_data/money_collector_input_test.txt");        
        auto money_collector = addComponent<MoneyCollector>("money_collector");
        // Couple the input reader to the money collector's input port
        addCoupling(input_reader->out, money_collector->money_in);
    }
};

int main() {
    auto test_system = std::make_shared<MoneyCollectorTestSystem>("mc_test_system");

    cadmium::RootCoordinator rootCoordinator(test_system);
    
    rootCoordinator.setLogger<cadmium::STDOUTLogger>(";");

    rootCoordinator.start();
    rootCoordinator.simulate(25.0);
    rootCoordinator.stop();

    return 0;
}