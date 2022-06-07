#pragma once
#include <map>
#include <memory>
#include <vector>
#include "runtime/model/game-state.h"

class GameStateContainer {
    public:
        ~GameStateContainer();
        std::pair<uint8_t, GameState*> create();
        [[nodiscard]] bool has(uint8_t key) const;
        GameState* get(uint8_t key);
        std::vector<std::pair<uint8_t, GameState*>> getAll();
    private:
        std::map<uint8_t, GameState*> m_states {};
    private:
        uint8_t getNext() const;
};