#pragma once

#include <cadmium/modeling/devs/coupled.hpp>
#include "message_queue.hpp"
#include "central_controller.hpp"

namespace presto {

    struct RechargeController : public cadmium::Coupled {
        // External input ports 
        cadmium::Port<double> money_input;
        cadmium::Port<double> card_input;
        cadmium::Port<double> receipt_request_input;

        // External output ports 
        cadmium::Port<double> balance_output;
        cadmium::Port<double> receipt_output;

        RechargeController(const std::string& id) : cadmium::Coupled(id) {
            // add components
            auto mq = addComponent<MessageQueue>("MessageQueue");
            auto cc = addComponent<CentralController>("CentralController");

            // Initialize ports
            money_input = addInPort<double>("money_input");
            card_input = addInPort<double>("card_input");
            receipt_request_input = addInPort<double>("receipt_request_input");
            balance_output = addOutPort<double>("balance_output");
            receipt_output = addOutPort<double>("receipt_output");

            // External input couplings to map the coupled model's input ports to the internal components
            addCoupling(money_input, mq->money_in);
            addCoupling(card_input, mq->card_in);
            addCoupling(receipt_request_input, mq->receipt_request_in);

            // Internal couplings to connect the MessageQueue and CentralController together
            // Send message to controller
            addCoupling(mq->message_out, cc->message_in);
            // Handshake acknowledgment
            addCoupling(cc->queue_out, mq->done);

            // External output couplings to map the internal components output ports to the coupled model's output ports
            addCoupling(cc->control_balance_out, balance_output);
            addCoupling(cc->control_receipt_out, receipt_output);
        }
    };

} // namespace presto