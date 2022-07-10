#pragma once
#include <map>
#include <memory>
#include <vector>
#include "runtime/model/game-state.h"

class GameStateContainer {
    public:
        ~GameStateContainer();
        /**
         * Creates new state and returns key:state pair
         * @return
         */
        std::pair<uint8_t, GameState*> create();
        /**
         * Returns true if state exists
         * @param key Key for state lookup
         * @return
         */
        [[nodiscard]] bool has(uint8_t key) const;
        /**
         * Returns state with matching key
         * @param key Key to match
         * @return
         */
        GameState* get(uint8_t key);
        /**
         * Returns all states and keys
         * @return
         */
        std::vector<std::pair<uint8_t, GameState*>> getAll();
    private:
        std::map<uint8_t, GameState*> m_states {};
        std::vector<uint8_t> m_statesToDestroy {};
};