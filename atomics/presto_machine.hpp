#pragma once

#include <cadmium/modeling/devs/coupled.hpp>
#include "money_collector.hpp"
#include "card_reader.hpp"
#include "receipt_processor.hpp"
#include "balance_display.hpp"
#include "recharge_controller.hpp"

namespace presto {

    struct PrestoCardChargingMachine : public cadmium::Coupled {
        // External Input Ports
        cadmium::Port<double> money_in;
        cadmium::Port<double> card_in;
        cadmium::Port<double> request_in;

        // External Output Ports
        cadmium::Port<double> balance_out;
        cadmium::Port<double> request_out;

        PrestoCardChargingMachine(const std::string& id) : cadmium::Coupled(id) {
            // Components
            auto mc = addComponent<MoneyCollector>("MoneyCollector");
            auto cr = addComponent<CardReader>("CardReader");
            auto rp = addComponent<ReceiptProcessor>("ReceiptProcessor");
            auto bd = addComponent<BalanceDisplay>("BalanceDisplay");
            auto controller = addComponent<RechargeController>("RechargeController");

            // Initialize ports
            money_in = addInPort<double>("money_in");
            card_in = addInPort<double>("card_in");
            request_in = addInPort<double>("request_in");
            balance_out = addOutPort<double>("balance_out");
            request_out = addOutPort<double>("request_out");

            // External input couplings
            addCoupling(money_in, mc->money_in);
            addCoupling(card_in, cr->cardinfo_in);
            addCoupling(request_in, rp->request_in);

            // Internal couplings
            addCoupling(mc->money_out, controller->money_input);
            addCoupling(cr->cardinfo_out, controller->card_input);
            addCoupling(rp->request_out, controller->receipt_request_input);
            
            // Controller logic back to hardware
            addCoupling(controller->balance_output, bd->balance_in);
            addCoupling(controller->receipt_output, rp->central_control_in);

            // External output couplings to map the hardware outputs to the physical machine boundaries
            addCoupling(bd->balance_out, balance_out);
            addCoupling(rp->receipt_out, request_out);
        }
    };

}