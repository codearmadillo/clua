#pragma once
#include <string>
#include <fstream>

namespace utils {
    const char* read_file(const char* path) {
        std::string line, output;
        std::ifstream in(path);
        while(std::getline(in, line)) {
            output += line + "\n";
        }
        return output.c_str();
    }
}