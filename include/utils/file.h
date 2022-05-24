#pragma once
#include <string>
#include <fstream>

namespace utils {
    std::string read_file(const char* path) {
        std::string out;
        std::ifstream in(path);
        for(std::string line; getline( in, line ); )
        {
            out.append(line);
        }
        return out;
    }
}