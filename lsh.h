#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <thread>

#include "dict_acgt.h"
#include "cgk.h"
#include "h.h"
#include "f.h"
#include "data.h"
#include "runner.h"
#include "log.h"

class LSH {

public:
    // r - number of different embeddings
    // z - number of hash functions
    // m - size of hash function
    // word_len - avg size of word in dataset
    LSH (int _r, int _z, int _m, int _word_len, int _threads_num = 1) {
        r = _r;
        z = _z;
        m = _m;
        threads_num = _threads_num;

        // each embedding is processed as distinct runner
        for (int i = 0; i != r; ++i) {
            L(linfo)<< "creating runner for embedding";
            Runner runner(_word_len, z, m, 70);
            runners.push_back(runner);
        }

        L(linfo)<< "created " << runners.size() << " runners";
    }


    std::vector<std::vector<int>> get_clusters() {
        std::vector<std::unordered_set<int>> clusters;

        std::vector<std::thread> threads;

        // compute each embedding
        for (int i = 0; i != r; ++i) {
            std::thread t(execute_runner, runners[i]);
            threads.push_back(std::move(t));

            if (threads.size() == threads_num || i == z - 1) {
                for (int th_num = 0; th_num != threads.size(); ++th_num) {
                    threads[th_num].join();
                }
                threads.clear();
            }
        }

        // for (const auto& cluster : clusters) {
        //     for (int c : cluster) {
        //         std::cout << c << " ";
        //     }
        //     std::cout << std::endl;
        // }

        return {};
    }

private:
    int r;
    int z;
    int m;
    int threads_num;
    std::vector<Runner> runners;

    static void execute_runner(Runner runner) {
        for (int i = 0; i != data.size(); ++i) {
            if (i % 1000 == 0) {
                std::cout << i << std::endl;
            }
            runner.run(i);
        }

        runner.log_result();

        const auto prediction = runner.get_results();
        L(ldebug) << "Prediction: " << prediction.get_log();

        // write to file
    }


};
