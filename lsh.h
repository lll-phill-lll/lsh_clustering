#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <future>

#include "dict_acgt.h"
#include "cgk.h"
#include "h.h"
#include "f.h"
#include "data.h"
#include "runner.h"
#include "log.h"
#include "prediction.h"

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
            Runner runner(_word_len, z, m, 50);
            runners.push_back(runner);
        }

        L(linfo)<< "created " << runners.size() << " runners";
    }


    Prediction get_clusters() {
        std::vector<Prediction> predictions;

        std::vector<std::future<Prediction>> futures;

        // compute each embedding
        for (int i = 0; i != r; ++i) {
            futures.push_back( std::async(std::launch::async, execute_runner, runners[i]));

            if (futures.size() == threads_num || i == r - 1) {
                for (int future_num = 0; future_num != futures.size(); ++future_num) {
                    predictions.push_back(futures[future_num].get());
                }
                futures.clear();
            }
        }
        Intersector intersector(0);

        return intersector.intersect(predictions, data.size());

    }

private:
    int r;
    int z;
    int m;
    int threads_num;
    std::vector<Runner> runners;

    static Prediction execute_runner(Runner runner) {
        for (int i = 0; i != data.size(); ++i) {
            if (i % 1000 == 0) {
                std::cout << i << std::endl;
            }
            runner.run(i);
        }

        runner.log_result();

        const auto prediction = runner.get_results();
        L(ldebug) << "Prediction: " << prediction.get_log();
        return prediction;

        // write to file
    }
};
