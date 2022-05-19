#include <iostream>

// #include "example.h"
#include "lsh.h"
#include "data.h"
#include "log.h"

void set_logger() {
    initLogger( "logfile.log", ldebug);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./lsh <path to file>" << std::endl;
        exit(1);
    }
    std::srand(std::time(nullptr));
    set_logger();

    data = Data(argv[1]);

    LSH lsh(20, 20, 40, 5000);

    const auto& prediction = lsh.get_clusters();
    L(linfo) << "Final result:\n" << prediction.get_log();
}
