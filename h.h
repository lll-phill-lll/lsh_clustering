#pragma once

#include <string>

class H {
public:
    H(int pos): pos(pos) {};

    char get_hash(const std::string& s) const {
        assert(s.length() > pos);
        return s[pos];
    }

private:
    int pos;
};

