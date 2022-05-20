#pragma once

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "dict.h"
#include "data.h"
#include "log.h"

template <class Dict>
class CGK {
public:
    CGK() = default;

    // receives len of word and alphabet size
    CGK(int len, int id = 0, std::vector<std::vector<int>> random_function = {}) {
        N = len;
        alphabet_size = dict.size();
        embedding_len = N * embedding_multiplier;
        runner_id = id;
        if (random_function.size() == 0) {
            generate_random_function();
        } else {
            R = random_function;
        }

        LN(ldebug, runner_id) << get_init_state();
    }

    std::string compute(std::string_view input) {
        std::string res;
        res.resize(embedding_len);
        int i = 0;
        for (int j = 0; j != embedding_len; ++j) {
            char c = dict.get_extra();
            if (i < input.size()) {
                c = input[i];
            }
            res[j] = c;
            // deal with arbitrary letters
            if (dict[c] < R.size()) {
                i += R[dict[c]][j];
            } else {
                // if we have extra symbol just increase counter
                ++i;
            }

        }

        LN(ldebug, runner_id) << "embedding for string\n\t" << input << "\n\tis\n\t" << res;
        return res;
    }

private:
    // length of input string
    int N;
    // number of letters in alphabet
    int alphabet_size;

    // random function
    std::vector<std::vector<int>> R;

    // Dict that maps chars to int
    Dict dict;

    // input string of size N will be mapped into embedding multiplier * N string
    int embedding_multiplier = 3;

    // multiplier * N
    int embedding_len;

    // id of runner that created this embedding
    int runner_id;

    void generate_random_function() {
        R.resize(alphabet_size, std::vector<int>(embedding_len));
        for (int i = 0; i != alphabet_size; ++i) {
            for (int j = 0; j != embedding_len; ++j) {
                int percent = std::rand() % 100;
                R[i][j] = (percent > 60) ? 1 : 0;
            }
        }
    }

    std::string get_init_state() const {
        std::stringstream buffer;
        buffer << "Initialized CGK embedding\n";
        buffer << "\t alphabet_size: " << alphabet_size << "\n";
        buffer << "\t input string len: " << N << "\n";
        buffer << "\t random string:\n";
        buffer << "\t\t";
        for (int i = 0; i != embedding_len; ++i) {
            buffer << "\t" << i + 1;
        }

        buffer << "\n";

        for (int i = 0; i != alphabet_size; ++i) {
            buffer << "\t\t";
            buffer << dict[i] << ":";
            for (int j = 0; j != embedding_len; ++j) {
                buffer << "\t" << R[i][j];
            }
            buffer << "\n";
        }

        return buffer.str();
    }
};

