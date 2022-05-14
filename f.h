#pragma once

#include <string>
#include <vector>

#include "h.h"
#include "constants.h"

class F {
public:
    F(std::vector<H> hash_functions): hash_functions(hash_functions) {};

    std::string get_hash_composition(const std::string& s) const {
        std::string result;
        result.reserve(hash_functions.size());

        for (const auto& h : hash_functions) {
            char next_hash_letter = h.get_hash(s);
            if (next_hash_letter != EXTRA_BASE) {
                result.push_back(next_hash_letter);
            }
        }

        return result;
    }

    friend std::ostream& operator<<(std::ostream &os, const F& f);

private:
    std::vector<H> hash_functions;

};

std::ostream& operator<<(std::ostream& os, const F& f) {
    for (const auto& h : f.hash_functions) {
        os << h << " ";
    }
    return os;
}
