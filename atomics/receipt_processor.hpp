#pragma once

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>

namespace presto {

    enum class ReceiptPhase { PASSIVE, REQUESTING, PRINTING };
    
    // State Definition
    struct ReceiptProcessorState {
        ReceiptPhase phase;
        int card_no;
        double sigma;

        ReceiptProcessorState() : 
            phase(ReceiptPhase::PASSIVE), 
            card_no(0), 
            sigma(std::numeric_limits<double>::infinity()) {}
    };

    // Logging
    inline std::ostream& operator<<(std::ostream& out, const ReceiptProcessorState& state) {
        std::string phase_str = (state.phase == ReceiptPhase::PASSIVE) ? "passive" : 
                                (state.phase == ReceiptPhase::REQUESTING) ? "requesting" : "printing";
        out << phase_str << ", card_no: " << state.card_no;
        return out;
    }

    // Atomic model definition
    class ReceiptProcessor : public cadmium::Atomic<ReceiptProcessorState> {
    public:
        cadmium::Port<double> request_in;
        cadmium::Port<double> central_control_in;
        cadmium::Port<double> request_out;
        cadmium::Port<double> receipt_out;

        ReceiptProcessor(const std::string& id) : cadmium::Atomic<ReceiptProcessorState>(id, ReceiptProcessorState()) {
            request_in = addInPort<double>("request_in");
            central_control_in = addInPort<double>("central_control_in");
            request_out = addOutPort<double>("request_out");
            receipt_out = addOutPort<double>("receipt_out");
        }

        // Time advance function
        double timeAdvance(const ReceiptProcessorState& state) const override {
            return state.sigma;
        }

        // Internal transition function
        void internalTransition(ReceiptProcessorState& state) const override {
            state.phase = ReceiptPhase::PASSIVE;
            state.sigma = std::numeric_limits<double>::infinity();
        }

        // External transition function
        void externalTransition(ReceiptProcessorState& state, double e) const override {
            state.sigma -= e;
            
            if (!request_in->getBag().empty()) {
                state.phase = ReceiptPhase::REQUESTING;
                state.sigma = 0.0;
            } else if (!central_control_in->getBag().empty()) {
                state.card_no = (int)central_control_in->getBag().back();
                state.phase = ReceiptPhase::PRINTING;
                 // 2s printing delay before outputting the receipt
                state.sigma = 2.0;
            }
        }

        // Output function
        void output(const ReceiptProcessorState& state) const override {
            // When requesting a receipt output a -1 to signal the controller and when printing output the card number as the receipt content
            if (state.phase == ReceiptPhase::REQUESTING) {
                request_out->addMessage(-1);
            } else if (state.phase == ReceiptPhase::PRINTING) {
                receipt_out->addMessage(state.card_no);
            }
        }
    };

}