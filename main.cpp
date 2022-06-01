#ifndef NDEBUG
    // Production builds should set NDEBUG=1
    #define NDEBUG false
#else
    #define NDEBUG true
#endif
#ifndef DEBUG
    #define DEBUG !NDEBUG
#endif

#include "clua.h"

int main() {
    Clua c;
    return 0;
}



