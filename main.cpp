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

    const auto& prediction = lsh.get_clusters();
    L(ldebug) << "Final result:\n" << prediction.get_log();

    std::ofstream outfile;
    outfile.open (config.output_file);
    outfile << prediction.get_log();
    outfile.close();

    delete data;
}
