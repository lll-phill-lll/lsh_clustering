#pragma once

#include "cli/CLI11.h"

class Config {
public:
    int Load(int argc, char** argv) {
        CLI::App app("LSH clustering");
        app.add_option("-i,--in_file,in_file", input_file, "Input file name")->check(CLI::ExistingFile)->required();
        app.add_option("-z,--z,z", z, "Number of hash functions")->required();
        app.add_option("-r,--r,r", r, "Number of different embeddings")->required();
        app.add_option("-m,--m,m", m, "Size of hash function")->required();
        app.add_option("-w,--word_len,word_len", word_len, "Average word length in dataset")->required();

        app.add_option("-o,--out_file,out_file", output_file, "Output file name")->capture_default_str();
        app.add_option("--hierarch,hierarch", hierarch_mode, "Enable hierarchical mode of clustering")->capture_default_str();
        app.add_option("-t,--thread_num,thread_num", thread_num, "Number of threads")->capture_default_str();

        app.add_option("--runner_consensus_threshold,runner_consensus_threshold", runner_consensus_threshold, "What percent of runners must agree on a cluster to save it to final result: 0 - each final cluster is a union of all embedding's guesses, 100 - each final cluster is a interssection of all embedding's guesses")->capture_default_str();
        app.add_option("--embedding_consensus_threshold,embedding_consensus_threshold", embedding_consensus_threshold, "what percent must agree on a cluster to save it to final result same as runner_consensus_threshold but consenssus inside single embedding across multiple hash funtions")->capture_default_str();

        CLI11_PARSE(app, argc, argv);
        return 0;
    }

    // z - number of hash functions
    int z;
    // r - number of different embeddings
    int r;
    // m - size of hash function
    int m;
    // word_len - avg size of word in dataset
    int word_len;
    // number of threads for executing process
    int thread_num = 1;
    // filename where to get input strings
    std::string input_file;
    // filename where to save output strings
    std::string output_file = "result.txt";
    // what percent of runners must agree on a cluster to save it to final result
    // 0 - each final cluster - union of all embedding's guesses
    // 100 - each final cluster - interssection of all embedding's guesses
    double runner_consensus_threshold = 0;
    // what percent must agree on a cluster to save it to final result
    // same as runner_consensus_threshold but consenssus inside single embedding across
    // multiple hash funtions
    //
    // what percent must agree on a cluster to save it to final result same as runner_consensus_threshold but consenssus inside single embedding across multiple hash funtions
    double embedding_consensus_threshold = 40;
    bool hierarch_mode = false;
};
