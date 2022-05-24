#pragma once

#include <iostream>
#include <vector>
#include <sstream>

namespace utils {
    std::vector<std::string> string_explode(std::string const& source, char const delimiter) {
        std::vector<std::string> parts;
        std::istringstream iss(source);

        for (std::string token; std::getline(iss, token, delimiter); ) {
            parts.push_back(std::move(token));
        }

        return parts;
    }
}