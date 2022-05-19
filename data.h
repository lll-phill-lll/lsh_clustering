#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

// mmap, munmap
#include <sys/mman.h>
// fstat
#include <sys/stat.h>
// open
#include <fcntl.h>
// close
#include <unistd.h>

#include "log.h"

class Data {
private:
    size_t lines = 0;
    int fd;
    char* file_map;
    off_t input_size;
    std::vector<off_t> strings_starts;


public:
    Data() = default;

    Data(const std::string& path) {

        fd = open(path.c_str(), O_RDONLY);
        if (fd == -1) {
            perror("can't open");
            exit(1);
        }

        struct stat st;
        if (fstat(fd, &st) == -1) {
            perror("can't stat");
            exit(1);
        }

        input_size = st.st_size;
        void* mp = mmap(NULL, input_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (file_map == MAP_FAILED) {
            perror("mmap error");
            exit(1);
        }

        file_map = static_cast<char*>(mp);

        strings_starts.push_back(0);
        std::cout << input_size << std::endl;
        for (off_t i = 0; i != input_size; ++i) {
            if (file_map[i] == '\n') {
                std::cout << "ind: " << i + 1 << std::endl;
                strings_starts.push_back(i + 1);
                lines++;
            } else if (i == input_size - 1) {
                std::cout << "ind: " << i + 1 << std::endl;
                lines++;
                strings_starts.push_back(i + 1);
            }
        }

        std::cout << "lines: " << lines << std::endl;

        for (int i = 0; i != lines; ++i) {
            std::cout << (*this)[i] << std::endl;
        }

        L(linfo) << "Data reading finished: " << lines << " lines";
    }

    int get_len(int ind) const {
        off_t start_pos = strings_starts[ind];
        off_t len = strings_starts[ind + 1] - start_pos - 1;
        return len;
    }

    char get_char(int ind, int pos) const {
        off_t start_pos = strings_starts[ind];
        return file_map[start_pos + pos];
    }

    std::string_view operator[](int ind) const {
        off_t start_pos = strings_starts[ind];
        off_t len = strings_starts[ind + 1] - start_pos - 1;
        std::cout << "data:\nstart: "<<start_pos << " len: " << len << std::endl;
        return std::string_view(file_map + start_pos, len);
    }

    int size() {
        return lines;
    }

    ~Data() {
        if (munmap(file_map, input_size) == -1) {
            perror("error");
            exit(1);
        }

        if (close(fd) == -1) {
            perror("error");
            exit(1);
        }
    }

};

Data* data;
