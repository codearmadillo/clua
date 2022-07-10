#include "runtime/container/game-state.h"
#include "utils/log.h"

GameStateContainer::~GameStateContainer() {
    for (auto i : m_states) {
        delete i.second;
    }
}

std::pair<uint8_t, GameState*> GameStateContainer::create() {
    auto id = m_states.size();
    m_states[id] = new GameState(id);
    return { id, m_states[id] };
}

bool GameStateContainer::has(uint8_t key) const {
    return m_states.count(key) > 0;
}

GameState* GameStateContainer::get(uint8_t key) {
    if (!has(key)) {
        return nullptr;
    }
    return m_states[key];
}

std::vector<std::pair<uint8_t, GameState*>> GameStateContainer::getAll() {
    std::vector<std::pair<uint8_t, GameState*>> out;

    for (auto i : m_states) {
        if (i.second) {
            out.emplace_back(i.first, i.second);
        }
    }

    return out;
}