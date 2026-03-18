#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
#include <cadmium/lib/iestream.hpp>
#include <memory>
#include "../atomics/receipt_processor.hpp"

using namespace cadmium;
using namespace presto;

struct ReceiptProcessorTestSystem : public Coupled {
    ReceiptProcessorTestSystem(const std::string& id) : Coupled(id) {
        auto input_reader = addComponent<lib::IEStream<double>>("input_reader", "input_data/receipt_processor_input_test.txt");
        auto receipt_processor = addComponent<ReceiptProcessor>("receipt_processor");
        
        // Couple the input reader to both the request_in and central_control_in ports
        addCoupling(input_reader->out, receipt_processor->request_in);
        addCoupling(input_reader->out, receipt_processor->central_control_in);
    }
};

int main() {
    auto test_system = std::make_shared<ReceiptProcessorTestSystem>("rp_test_system");
    cadmium::RootCoordinator rootCoordinator(test_system);
    rootCoordinator.setLogger<cadmium::STDOUTLogger>(";");
    rootCoordinator.start();
    rootCoordinator.simulate(10.0);
    rootCoordinator.stop();
    return 0;
}