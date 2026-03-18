#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
#include <cadmium/lib/iestream.hpp>
#include <memory>
#include "../atomics/balance_display.hpp"

using namespace cadmium;
using namespace presto;

struct BalanceDisplayTestSystem : public Coupled {
    BalanceDisplayTestSystem(const std::string& id) : Coupled(id) {
        auto input_reader = addComponent<lib::IEStream<double>>("input_reader", "input_data/balance_display_input_test.txt");
        auto balance_display = addComponent<BalanceDisplay>("balance_display");
        
        // Couple the input reader to the balance_display's input port
        addCoupling(input_reader->out, balance_display->balance_in);
    }
};

int main() {
    auto test_system = std::make_shared<BalanceDisplayTestSystem>("bd_test_system");
    cadmium::RootCoordinator rootCoordinator(test_system);
    rootCoordinator.setLogger<cadmium::STDOUTLogger>(";");

    rootCoordinator.start();
    rootCoordinator.simulate(15.0);
    rootCoordinator.stop();

    return 0;
}