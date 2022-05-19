#include <iostream>
#include "foundation/lua.h"

Lua::Lua(): m_state(luaL_newstate()) {
    LoadLibraries();
    SetGlobals();
}
Lua::~Lua() {
    lua_close(m_state);
}
lua_State* Lua::GetState() const {
    return m_state;
}
void Lua::Run(const char* source) {
    (void)luaL_loadstring(m_state, source);
    (void)lua_call(m_state, 0, 0);
}
void Lua::SetGlobals() {
    lua_newtable(m_state);

    lua_pushstring(m_state, "hello");
    lua_pushcfunction(m_state, [](lua_State* lua){
        std::cout << "hello from Clua engine!\n";
        return 0;
    });

    lua_settable(m_state, -3);
    lua_setglobal(m_state, m_globalTableName);
}
void Lua::LoadLibraries() {
    lua_gc(m_state, LUA_GCSTOP, 0);
    luaL_openlibs(m_state);
    lua_gc(m_state, LUA_GCRESTART, 0);
}
void Lua::SetFloat(const char* name, float value, const bool globalApi) {
    std::cout << "adding '" << (globalApi ? "clua." : "") << name << "' with value " << std::to_string(value) << "\n";
}