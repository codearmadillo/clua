#pragma once

#include "runtime/container/game-state.h"

class Runtime {
    public:
        static Runtime& getInstance() {
            static Runtime instance;
            return instance;
        }
        void onBeforeWindowStart();
        void onAfterWindowClose();
        void onWindowFrame();
        Runtime(Runtime const&)         = delete;
        void operator=(Runtime const&)  = delete;
        void setBindings();
        void loadUserSource();
    private:
        Runtime();
        ~Runtime();
        void setRuntimeBindings();
        void setStateBindings();
    private:
        GameStateContainer m_gameStateContainer;
};