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


    // std::string input;
    // std::cin >> input;

    // CGK<ACGT> cgk(input.size(), R1);

    // std::string res = cgk.compute(input);
    // std::cout << res << std::endl;


    // run_example();

    data = Data(argv[1]);

    LSH lsh(2, 2, 2, 10);

    const auto& clusters = lsh.get_clusters();


    for (const auto& cluster : clusters) {
        for (int index : cluster) {
            L(linfo) << index << " ";
        }
        L(linfo) << std::endl;
    }

}
