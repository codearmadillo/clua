#pragma once


#include <cstdint>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

class GameState {
    public:
        GameState(uint8_t id);
        ~GameState();
        void getRef();
    private:
        const uint8_t m_id;
        int m_ref;
    private:
        void setBindings();
};