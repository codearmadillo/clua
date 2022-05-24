#include <iostream>
#include <vector>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "foundation/lua.h"

const std::string lua_script = R"(
    print(clua.engine.version)
)";


int main() {

    lua_pushnumber(Lua::getInstance().rawState(), 0.1);

    Lua::getInstance().bind("engine.version");
    Lua::getInstance().pcall(lua_script.c_str(), 0, 0);

    return 0;
}
