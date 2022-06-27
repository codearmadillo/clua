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
#include "foundation/lua.h"
#include <iostream>

#include "runtime/container/game-state.h"

/*
struct Foo {
    void draw() const {
        std::cout << "draw";
    }
    void bind() {
        Lua::pushvoidptr(this);
        Lua::pushcclosure([](lua_State* l){
            const Foo* p = (const Foo*) lua_topointer(l, lua_upvalueindex(1));
            p->draw();
            return 0;
        }, 1);
    }
};
 */

int main() {

    Clua c;
    return 0;

}



