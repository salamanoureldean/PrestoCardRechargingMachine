#pragma once

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>

namespace presto {

    // State Definition
    struct CardReaderState {
        bool is_busy;
        int card_id;
        double sigma;

        CardReaderState() : 
            is_busy(false), 
            card_id(0), 
            sigma(std::numeric_limits<double>::infinity()) {}
    };

    inline std::ostream& operator<<(std::ostream& out, const CardReaderState& state) {
        out << (state.is_busy ? "busy" : "passive") 
            << ", card_id: " << state.card_id;
        return out;
    }

    // Atomic Model Definition
    class CardReader : public cadmium::Atomic<CardReaderState> {
    public:
        cadmium::Port<double> cardinfo_in;
        cadmium::Port<double> cardinfo_out;

        CardReader(const std::string& id) : cadmium::Atomic<CardReaderState>(id, CardReaderState()) {
            cardinfo_in = addInPort<double>("cardinfo_in");
            cardinfo_out = addOutPort<double>("cardinfo_out");
        }

        double timeAdvance(const CardReaderState& state) const override {
            return state.sigma;
        }

        void internalTransition(CardReaderState& state) const override {
            if (state.is_busy) {
                state.is_busy = false;
                state.sigma = std::numeric_limits<double>::infinity();
            }
        }

        void externalTransition(CardReaderState& state, double e) const override {
            state.sigma -= e;

            if (!state.is_busy && !cardinfo_in->getBag().empty()) {
                // Generate a random 3 digit card number between 100 and 999
                state.card_id = 100 + (std::rand() % 900);
                
                state.is_busy = true;
                state.sigma = 1.0; 
            }
        }

        void output(const CardReaderState& state) const override {
            if (state.is_busy) {
                cardinfo_out->addMessage(state.card_id);
            }
        }
    };

}