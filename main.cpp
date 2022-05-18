#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

const std::string lua_static_script = R"(
    print('Hello from Lua');
)";

class Module {
    public:
        virtual void DefineLuaBindings() = 0;
};
class SoundModule: public Module {
    public:
        void DefineLuaBindings() override {
            std::cout << "Lua for sound\n";
        }
};

class LuaModule {
    public:
        LuaModule(): m_state(luaL_newstate()) {
            lua_gc(m_state, LUA_GCSTOP, 0);
            luaL_openlibs(m_state);
            lua_gc(m_state, LUA_GCRESTART, 0);
        }
        ~LuaModule() {
            lua_close(m_state);
        }
        [[nodiscard]] lua_State* GetLuaState() const {
            return m_state;
        }
    private:
        lua_State* m_state;
};

int main() {

    LuaModule lua;

    (void)luaL_loadstring(lua.GetLuaState(), lua_static_script.c_str());
    (void)lua_call(lua.GetLuaState(), 0, 0);

    return 0;
}
