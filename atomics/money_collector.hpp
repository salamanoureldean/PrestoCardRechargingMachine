#pragma once

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>

namespace presto {

    // State Definition
    struct MoneyCollectorState {
        bool is_busy;          
        double money_amount;   
        double sigma;

        // Constructor initializes to  passive state
        MoneyCollectorState() : 
            is_busy(false), 
            money_amount(0.0), 
            sigma(std::numeric_limits<double>::infinity()) {}
    };

    // Logging
    inline std::ostream& operator<<(std::ostream& out, const MoneyCollectorState& state) {
        out << (state.is_busy ? "busy" : "passive") 
            << ", amount: " << state.money_amount;
        return out;
    }

    // Atomic model
    class MoneyCollector : public cadmium::Atomic<MoneyCollectorState> {
    public:
        // Ports
        cadmium::Port<double> money_in;
        cadmium::Port<double> money_out;

        // Constructor
        MoneyCollector(const std::string& id) : cadmium::Atomic<MoneyCollectorState>(id, MoneyCollectorState()) {
            money_in = addInPort<double>("money_in");
            money_out = addOutPort<double>("money_out");
        }

        // Time advance function 
        double timeAdvance(const MoneyCollectorState& state) const override {
            return state.sigma;
        }

        // Internal transition function
        void internalTransition(MoneyCollectorState& state) const override {
            // After 1s of processing revert to passive state and reset money amount
            if (state.is_busy) {
                state.is_busy = false;
                state.sigma = std::numeric_limits<double>::infinity();
            }
        }

        // External transition function
        void externalTransition(MoneyCollectorState& state, double e) const override {
            // Reduce the remaining time by the elapsed time first 
            state.sigma -= e;

            // Process inputs if the model is currently passive 
            if (!state.is_busy && !money_in->getBag().empty()) {
                // Grab the first inputted value from the bag
                const auto& msg = money_in->getBag()[0]; 
                
                state.money_amount = msg;
                state.is_busy = true;
                state.sigma = 1.0;    
            }
        }

        // Output function
        void output(const MoneyCollectorState& state) const override {
            // Only output if at the end of the busy phase 
            if (state.is_busy) {
                money_out->addMessage(state.money_amount);
            }
        }
    };

}