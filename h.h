#pragma once

#include <string>

class H {
public:
    H(int pos): pos(pos) {};

    char get_hash(const std::string& s) const {
        assert(s.length() > pos);
        return s[pos];
    }

    friend std::ostream& operator<<(std::ostream &os, const H& h);

private:
    int pos;
};

std::ostream& operator<<(std::ostream& os, const H& h) {
    os << h.pos;
    return os;
}

