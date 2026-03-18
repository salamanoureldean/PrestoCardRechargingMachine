#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
#include <cadmium/lib/iestream.hpp>
#include <memory>

#include "../atomics/central_controller.hpp"

using namespace cadmium;
using namespace presto;

struct CC_Test : public Coupled {
    CC_Test(const std::string& id) : Coupled(id) {
        auto msg_reader = addComponent<lib::IEStream<double>>(
            "msg_reader", "input_data/cc_messages.txt");

        auto cc = addComponent<CentralController>("central_controller");
        // Couple the message reader to the central controller's message_in port
        addCoupling(msg_reader->out, cc->message_in);
    }
};

int main() {
    auto test_system = std::make_shared<CC_Test>("cc_test");
    cadmium::RootCoordinator rootCoordinator(test_system);
    
    rootCoordinator.setLogger<cadmium::STDOUTLogger>(";");

    rootCoordinator.start();
    rootCoordinator.simulate(30.0);
    rootCoordinator.stop();

    return 0;
}