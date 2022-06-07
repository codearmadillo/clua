#pragma once

#include <cstdint>
#include <iostream>

class GameState {
    public:
        explicit GameState(uint8_t id);
        void setBindings();
        void pushToStack();
    private:
        const uint8_t m_id;
};