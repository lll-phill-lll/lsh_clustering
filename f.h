#pragma once

#include <string>
#include <vector>

#include "h.h"

class F {
public:
    F(std::vector<H> hash_functions): hash_functions(hash_functions) {};

    std::string get_hash_composition(const std::string& s) const {
        std::string result;
        result.reserve(hash_functions.size());

        for (const auto& h : hash_functions) {
            result.push_back(h.get_hash(s));
        }

        return result;
    }


private:
    std::vector<H> hash_functions;

};
