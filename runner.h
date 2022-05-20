#pragma once

#include "log.h"
#include "prediction.h"
#include "intersector.h"

struct Runner {
    CGK<ACGT> cgk;
    int word_len;
    int z, m;
    int id;
    std::vector<F> fs;
    std::vector<Prediction> predictions;
    double consensus_percent;
    int processed_words = 0;

    Runner(int _word_len, int _z, int _m, double _consensus_percent) {
        z = _z;
        m = _m;
        word_len = _word_len;
        consensus_percent = _consensus_percent;
        id = std::rand() % 1000;

        cgk = CGK<ACGT>(word_len, id);
        LN(linfo, id) << "Init runner:\n\tz: " << z << ", m: " << m << ", word_size: " << word_len;

        // initialize hash functions
        for (int f_num = 0; f_num != z; ++f_num) {
            std::vector<H> hs;
            for (int h_num = 0; h_num != m; ++h_num) {
                hs.push_back(H(rand() % (word_len * 3)));
            }
            fs.push_back(hs);
        }
        predictions.resize(z);
    }

    void run(int index) {
        const std::string emb = cgk.compute((*data)[index]);
        for (int i = 0; i != z; ++i) {
            const std::string hash_comp = fs[i].get_hash_composition(emb);

            LN(ldebug, id) << "RUN:\n\tinput:\t" << (*data)[index] << "\n\tembedding: " << emb << "\n\tf: " << fs[i] << "\n\thash: " << hash_comp;
            predictions[i].add(hash_comp, index);
        }
        ++processed_words;
    }

    Prediction get_results() {
        Intersector intersector(consensus_percent);
        LN(linfo, id) << "Started intersection";
        const auto result = intersector.intersect(predictions, processed_words);
        LN(linfo, id) << "Finished intersection";
        return result;
    }

    void log_result() {
        LN(ldebug, id) << get_log_result();
    }

    std::string get_log_result() const {
        std::stringstream buffer;
        buffer << "Possible clusters for F's:";
        for (int i = 0; i != fs.size(); ++i) {
            buffer << "\n\tf: " << fs[i] << "";
            buffer << predictions[i].get_log();
        }

        return buffer.str();
    }
};
