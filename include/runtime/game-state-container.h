#pragma once
#include <map>
#include <cstdint>
#include "game-state.h"

class GameStateContainer {
    public:
        static GameStateContainer& getInstance() {
            static GameStateContainer instance;
            return instance;
        }
        GameStateContainer(GameStateContainer const&) = delete;
        void operator=(GameStateContainer const&) = delete;
        GameState* createState();
    private:
        GameStateContainer() = default;
        ~GameStateContainer();
    private:
        std::map<uint16_t, GameState*> m_states;
};