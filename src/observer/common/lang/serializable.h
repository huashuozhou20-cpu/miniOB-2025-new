#pragma once
#include <iostream>
#include <string>

namespace common {
// Serializable interface
class Serializable {
public:
    virtual ~Serializable() = default;
    virtual int serialize(std::ostream &os) const { return 0; }
    virtual int deserialize(std::istream &is) { return 0; }
    virtual int get_serial_size() const { return 0; }
    virtual void to_string(std::string &output) const {}
};
}
