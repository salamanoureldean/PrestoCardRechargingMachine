#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/csv.hpp>
#include <cadmium/lib/iestream.hpp>
#include <memory>
#include "../atomics/message_queue.hpp"
#include <cadmium/simulation/logger/stdout.hpp>

using namespace cadmium;
using namespace presto;

struct MQ_Test : public Coupled {
    MQ_Test(const std::string& id) : Coupled(id) {
        // Setup input readers for each hardware port
        auto money_reader = addComponent<lib::IEStream<double>>(
            "money_reader", "input_data/mq_money.txt");

        auto card_reader = addComponent<lib::IEStream<double>>(
            "card_reader", "input_data/mq_card.txt");

        auto done_reader = addComponent<lib::IEStream<double>>(
            "done_reader", "input_data/mq_done.txt");

        auto mq = addComponent<MessageQueue>("message_queue");
        
        // Couple the readers to the message queue's input ports
        addCoupling(money_reader->out, mq->money_in);
        addCoupling(card_reader->out,  mq->card_in);
        addCoupling(done_reader->out,  mq->done);
    }
};

int main() {
    auto test_system = std::make_shared<MQ_Test>("mq_test");
    cadmium::RootCoordinator rootCoordinator(test_system);
    rootCoordinator.setLogger<cadmium::STDOUTLogger>(";");

    rootCoordinator.start();
    rootCoordinator.simulate(15.0);
    rootCoordinator.stop();

    return 0;
}