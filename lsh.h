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
#include "config.h"

class LSH {

public:
    // r - number of different embeddings
    // z - number of hash functions
    // m - size of hash function
    // word_len - avg size of word in dataset
    LSH (const Config& _config) {
        config = _config;


        // each embedding is processed as distinct runner
        for (int i = 0; i != config.r; ++i) {
            L(linfo)<< "creating runner for embedding";
            Runner runner(config.word_len, config.z, config.m, config.embedding_consensus_threshold);
            runners.push_back(runner);
        }

        L(linfo)<< "created " << runners.size() << " runners";
    }


    Prediction get_clusters() {
        std::vector<Prediction> predictions;

        std::vector<std::future<Prediction>> futures;

        // compute each embedding
        for (int i = 0; i != config.r; ++i) {
            futures.push_back( std::async(std::launch::async, execute_runner, runners[i]));

            if (futures.size() == config.thread_num || i == config.r - 1) {
                for (int future_num = 0; future_num != futures.size(); ++future_num) {
                    predictions.push_back(futures[future_num].get());
                }
                futures.clear();
            }
        }
        Intersector intersector(config.runner_consensus_threshold);

        return intersector.intersect(predictions, data.size());

    }

private:
    Config config;
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
