#pragma once
#include <iostream>

#if NDEBUG
    #define LOG_INFO(msg)
    #define LOG_ERROR(msg)
#else
    #define LOG_INFO(msg) { \
        std::cout << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; \
    }
    #define LOG_ERROR(msg) {                                              \
        std::cerr << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; \
    }
#endif

