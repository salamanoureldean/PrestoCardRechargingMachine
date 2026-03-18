#pragma once

#include <cadmium/modeling/devs/atomic.hpp>
#include <iostream>
#include <string>
#include <limits>
#include <queue>

namespace presto {

    // State Definition
    struct MessageQueueState {
        std::queue<double> messages;
        bool waiting_for_controller;
        double sigma;

        MessageQueueState() : 
            waiting_for_controller(false), 
            sigma(std::numeric_limits<double>::infinity()) {}
    };
    
    // Logging 
    inline std::ostream& operator<<(std::ostream& out, const MessageQueueState& state) {
            out << (state.waiting_for_controller ? "active" : "passive")
            << ", queue_size: " << state.messages.size();
        return out;
    }

    // Atomic Model Definition
    class MessageQueue : public cadmium::Atomic<MessageQueueState> {
    public:
        cadmium::Port<double> money_in;
        cadmium::Port<double> card_in;
        cadmium::Port<double> receipt_request_in;
        cadmium::Port<double> done;
        cadmium::Port<double> message_out;

        // Constructor
        MessageQueue(const std::string& id) : cadmium::Atomic<MessageQueueState>(id, MessageQueueState()) {
            money_in = addInPort<double>("money_in");
            card_in = addInPort<double>("card_in");
            receipt_request_in = addInPort<double>("receipt_request_in");
            done = addInPort<double>("done");
            message_out = addOutPort<double>("message_out");
        }

        // Time Advance function
        double timeAdvance(const MessageQueueState& state) const override {
            return state.sigma;
        }

        // Internal Transition function
        void internalTransition(MessageQueueState& state) const override {
            // After sending a message output we must stay locked until we receive a done ack from the controller
            state.sigma = std::numeric_limits<double>::infinity();
        }
        // External Transition function
        void externalTransition(MessageQueueState& state, double e) const override {
            state.sigma -= e;
            // Prioritize incoming messages over the done ack to ensure we don't miss any new requests while waiting for the controller
            for (const auto &msg : money_in->getBag())
                state.messages.push(msg);
            // Check the card and receipt request ports for new messages
            for (const auto &msg : card_in->getBag())
                state.messages.push(msg);
            // Receipt requests are represented as -1 in the message queue
            for (const auto &msg : receipt_request_in->getBag())
                state.messages.push(msg);
            // Check for done ack from controller
            if (!done->getBag().empty()) {
                state.waiting_for_controller = false;
                if (!state.messages.empty())
                    state.messages.pop();
            }
            // If we have messages to send and we're not currently waiting for the controller then trigger an output
            if (!state.messages.empty() && !state.waiting_for_controller) {
                state.waiting_for_controller = true;
                state.sigma = 0.0;
            } else {
                state.sigma = std::numeric_limits<double>::infinity();
            }
        }

        // Output function
        void output(const MessageQueueState& state) const override {
            if (state.waiting_for_controller && !state.messages.empty()) {
                message_out->addMessage(state.messages.front());
            }
        }
    
    };


}