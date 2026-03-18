#pragma once

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>

namespace presto {

// State Definition
struct CentralControllerState {
    bool is_busy;
    double currentBalance;
    int card_no;
    bool send_receipt;
    double sigma;

    CentralControllerState()
        : is_busy(false),
          currentBalance(0.0),
          card_no(0),
          send_receipt(false),
          sigma(std::numeric_limits<double>::infinity()) {}
};
// Required for logging the state to output files
inline std::ostream& operator<<(std::ostream& out, const CentralControllerState& s) {
    out << (s.is_busy ? "busy" : "passive")
        << ", bal: " << s.currentBalance
        << ", card: " << s.card_no
        << ", send_receipt: " << (s.send_receipt ? 1 : 0);
    return out;
}

// Atomic Model Definition
class CentralController : public cadmium::Atomic<CentralControllerState> {
public:
    cadmium::Port<double> message_in;
    cadmium::Port<double> control_balance_out;
    cadmium::Port<double> control_receipt_out;
    cadmium::Port<double> queue_out;

    CentralController(const std::string& id)
        : cadmium::Atomic<CentralControllerState>(id, CentralControllerState()) {
        message_in = addInPort<double>("message_in");
        control_balance_out = addOutPort<double>("control_balance_out");
        control_receipt_out = addOutPort<double>("control_receipt_out");
        queue_out = addOutPort<double>("queue_out");
    }

    // Time Advance function
    double timeAdvance(const CentralControllerState& s) const override {
        return s.sigma;
    }
    
    // Internal Transition function 
    void internalTransition(CentralControllerState& s) const override {
        if (s.is_busy) {
            s.is_busy = false;
            s.sigma = std::numeric_limits<double>::infinity();
            // Important: clear one-shot flag after processing
            s.send_receipt = false;
        }
    }

    void externalTransition(CentralControllerState& s, double e) const override {
        s.sigma -= e;

        // Ignore new messages while busy
        if (s.is_busy) return;

        if (!message_in->getBag().empty()) {
            // Take the first message or front
            const double v = message_in->getBag().front();

            // Interpret the message based on its value
            if (v == -1) {
                s.send_receipt = true;
            } else if (v > 0.0 && v <= 100.0) {
                s.currentBalance += v;
            } else if (v > 100.0) {
                s.card_no = static_cast<int>(v);
            }

            // After processing the message become busy for 1s to simulate preparation time
            s.is_busy = true;
            // Prep time
            s.sigma = 1.0; 
        }
    }
    
    // Output function
    void output(const CentralControllerState& s) const override {
        if (!s.is_busy) return;

        // Always output the current balance and a signal to the queue
        control_balance_out->addMessage(s.currentBalance);
        queue_out->addMessage(1.0);

        // Output the card number if receipt needed
        if (s.send_receipt) {
            control_receipt_out->addMessage(static_cast<double>(s.card_no));
        }
    }
};

} // namespace presto