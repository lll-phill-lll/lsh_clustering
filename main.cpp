#include <iostream>
#include <fstream>

#include "lsh.h"
#include "data.h"
#include "log.h"
#include "config.h"

void set_logger() {
    initLogger( "logfile.log", linfo);
}

Prediction run_lsh(Config& config) {
    std::unordered_set<int> idxs_to_process;
    for (int i = 0; i != data->size(); ++i) {
        idxs_to_process.insert(i);
    }

    Prediction prediction;

    if (config.hierarch_clusters) {
        // predictions.add_cluster(idxs_to_process);

        // while (prediction.get_clusters_num() < hierarch_clusters) {
        //     LSH lsh(config);

        //     prediction = lsh.get_clusters(idxs_to_process);
        //     std::vector<std::unordered_set<int>> clusters = prediction.get_clusters();
        //     config.m += 2;
        // }
    } else {
        LSH lsh(config);
        return lsh.get_clusters(idxs_to_process);

    }
    return prediction;
}

int main(int argc, char** argv) {
    Config config;
    if (config.Load(argc, argv) != 0) {
        exit(1);
    }

    std::srand(std::time(nullptr));

    set_logger();

    data = new Data(config.input_file);

    Prediction prediction = run_lsh(config);

    L(ldebug) << "Final result:\n" << prediction.get_log();

    std::ofstream outfile;
    outfile.open (config.output_file);
    outfile << prediction.get_log();
    outfile.close();

    delete data;
}
