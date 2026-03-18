#pragma once

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>

namespace presto {
    // State definition
    struct BalanceDisplayState {
        bool is_busy;
        double balance;
        double sigma;
        
        // Constructor initializes to passive state
        BalanceDisplayState() : 
            is_busy(false), 
            balance(0.0), 
            sigma(std::numeric_limits<double>::infinity()) {}
    };

    // Logging
    inline std::ostream& operator<<(std::ostream& out, const BalanceDisplayState& state) {
        out << (state.is_busy ? "busy" : "passive") 
            << ", balance: " << state.balance;
        return out;
    }

    // Atomic model definition
    class BalanceDisplay : public cadmium::Atomic<BalanceDisplayState> {
    public:
        cadmium::Port<double> balance_in;
        cadmium::Port<double> balance_out;

        BalanceDisplay(const std::string& id) : cadmium::Atomic<BalanceDisplayState>(id, BalanceDisplayState()) {
            balance_in = addInPort<double>("balance_in");
            balance_out = addOutPort<double>("balance_out");
        }

        // Time advance function
        double timeAdvance(const BalanceDisplayState& state) const override {
            return state.sigma;
        }

        // Internal transition function
        void internalTransition(BalanceDisplayState& state) const override {
            if (state.is_busy) {
                state.is_busy = false;
                state.sigma = std::numeric_limits<double>::infinity();
            }
        }

        // External transition function
        void externalTransition(BalanceDisplayState& state, double e) const override {
            state.sigma -= e;
            
            // Set sigma to 0 for an instant internal transition
            if (!balance_in->getBag().empty()) {
                // Grab the latest value from the input bag
                state.balance = balance_in->getBag().back();
                state.is_busy = true;
                state.sigma = 0.0;
            }
        }

        // Output function
        void output(const BalanceDisplayState& state) const override {
            if (state.is_busy) {
                balance_out->addMessage(state.balance);
            }
        }
    };

}