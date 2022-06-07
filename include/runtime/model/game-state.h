#pragma once

#include <cstdint>
#include <iostream>

class GameState {
    public:
        explicit GameState(uint8_t id);
    private:
        void setBindings();
    private:
        const uint8_t m_id;
        int m_ref;
};