#pragma once
#include <iostream>

namespace presto {
    struct PrestoMessage {
        double value;
        int type;

        PrestoMessage() : value(0), type(0) {}
        PrestoMessage(double v, int t) : value(v), type(t) {}
    };

    inline std::ostream& operator<<(std::ostream& out, const PrestoMessage& m) {
        out << "{" << m.value << "," << m.type << "}";
        return out;
    }
}