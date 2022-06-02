#pragma once
#include <iostream>

#if NDEBUG
    #define LOG_INFO(msg)
    #define LOG_ERROR(msg)
    #define LOG_WARN(msg)
#else
    #define LOG_WARN(msg) { \
        std::cout << "[WARN] " << __FILE __ << ":" << __LINE__ << " - " << msg << "\n"; \
    }
    #define LOG_INFO(msg) { \
        std::cout << "[INFO] " << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; \
    }
    #define LOG_ERROR(msg) {        \
        std::stringstream ss;       \
        ss << "[ERROR] " << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; \
        throw std::runtime_error(ss.str());\
    }
#endif

