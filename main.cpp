#include <iostream>
#include <fstream>

#include "lsh.h"
#include "data.h"
#include "log.h"
#include "config.h"

void set_logger() {
    initLogger( "logfile.log", ldebug);
}

int main(int argc, char** argv) {
    Config config;
    if (config.Load(argc, argv) != 0) {
        exit(1);
    }

    std::srand(std::time(nullptr));

    set_logger();

    data = new Data(config.input_file);

    LSH lsh(config);

    std::unordered_set<int> idxs_to_process;
    for (int i = 0; i != data->size(); ++i) {
        idxs_to_process.insert(i);
    }
    const auto& prediction = lsh.get_clusters(idxs_to_process);
    L(ldebug) << "Final result:\n" << prediction.get_log();

    std::ofstream outfile;
    outfile.open (config.output_file);
    outfile << prediction.get_log();
    outfile.close();

    delete data;
}
