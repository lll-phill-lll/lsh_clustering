#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "log.h"

class Data {
private:
    std::vector<std::string> data;
    size_t lines = 0;


public:
    Data() = default;

    Data(const std::string& path) {
        std::ifstream fin(path);
        std::string line;

        L(linfo) << "Data reading started";

        while (std::getline(fin, line)) {
            data.push_back(line);
            ++lines;
        }

        L(linfo) << "Data reading finished: " << lines << " lines";


    }

    std::string operator[](int ind) const {
        assert(ind < lines);
        return data[ind];
    }

    int size() {
        return data.size();
    }

};

Data data;
