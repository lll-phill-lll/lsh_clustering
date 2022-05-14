#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "dict_acgt.h"
#include "cgk.h"
#include "h.h"
#include "f.h"

const std::vector<std::vector<int>> R1 = {
    {0, 1, 0, 0, 1, 0, 1, 1, 0, 1, /*example end*/ 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 1, 0, 1, 1, 1, 1, 0, 0, 0, /*example end*/ 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 1, 1, 1, /*example end*/ 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 0, 1, /*example end*/ 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1},
};

const std::vector<std::vector<int>> R2 = {
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, /*example end*/ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {1, 1, 0, 0, 1, 1, 1, 1, 0, 0, /*example end*/ 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
    {1, 0, 1, 1, 0, 0, 1, 1, 0, 1, /*example end*/ 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 1, 1, 1, 0, 0, /*example end*/ 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0},
};

const std::string S1 = "ACGTGACGTG";
const std::string S2 = "ACGTCGCGTG";
const std::string S3 = "ACTTACCTG";
const std::string S4 = "ATCGATCGGT";

const int input_size = 10;

void run_example() {

    H h2(1);
    H h4(3);
    H h9(8);
    H h1(3);
    H h5(4);
    H h7(6);
    H h3(2);

    F f11({h2, h9});
    F f12({h1, h4});
    F f21({h2, h5});
    F f22({h7, h3});


    // iterate over r
    int r_num = 1;
    for (const auto& R : {R1, R2}) {
        CGK<ACGT> cgk(10, R);

        int s_num = 1;
        for (const auto& S : {S1, S2, S3, S4}) {
            if (r_num == 1) {
                std::string res = cgk.compute(S);
                // std::cout << res << std::endl;

                int f_num = 1;
                for (const auto& f : {f11, f12}) {
                    std::cout << "f" << r_num << f_num << ", s" << s_num << ": " << std::endl;
                    std::cout << res << std::endl;
                    std::cout << f.get_hash_composition(res) << std::endl;

                    ++f_num;
                }
            }
            ++s_num;
        }
        ++r_num;
    }
}
