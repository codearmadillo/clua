#pragma once

#include <cstdint>
#include <iostream>

class GameState {
    public:
        explicit GameState(uint8_t id);
        void process_update();
        void process_draw();
    private:
        void setBindings();
        void setAsActive();
        void setAsDefault();
        void onActivated();
        void onDeactivated();
    private:
        const uint8_t m_id;
        int m_ref;
        friend class Runtime;
};