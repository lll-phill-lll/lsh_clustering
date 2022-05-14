#pragma once

#include <iostream>

#include "dict.h"

class ACGT : public IDict {
public:

    int size() const {
        return 4;
    }

    char get_extra() const {
        return EXTRA;
    }


    int operator[](char c) const {
        switch (c) {
            case A: return 0;
            case C: return 1;
            case G: return 2;
            case T: return 3;
            case EXTRA: return 4;

            default:
                std::cerr << "[WARN] unknown char: " << c << std::endl;
                return -1;
        }
    }

    char operator[](int i) const {
        switch (i) {
            case 0: return A;
            case 1: return C;
            case 2: return G;
            case 3: return T;
            case 4: return EXTRA;

            default:
                std::cerr << "[WARN] unknown int: " << i << std::endl;
                return -1;
        }
    }

private:
    static constexpr char A = 'A';
    static constexpr char C = 'C';
    static constexpr char G = 'G';
    static constexpr char T = 'T';
    static constexpr char EXTRA = 'N';


};
