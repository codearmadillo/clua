#include <iostream>
#include "foundation/lua.h"

Lua::Lua(): m_state(luaL_newstate()) {
    LoadLibraries();
}
Lua::~Lua() {
    lua_close(m_state);
}
lua_State* Lua::GetState() const {
    return m_state;
}
void Lua::LoadLibraries() {
    lua_gc(m_state, LUA_GCSTOP, 0);
    luaL_openlibs(m_state);
    lua_gc(m_state, LUA_GCRESTART, 0);
}