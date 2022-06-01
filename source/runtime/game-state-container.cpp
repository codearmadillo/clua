#include <iostream>
#include "runtime/game-state-container.h"

GameState* GameStateContainer::createState() {
    unsigned int stateCount = m_states.size();
    m_states[stateCount] = new GameState(stateCount);
    return m_states.at(stateCount);
}
GameStateContainer::~GameStateContainer() {
    for(auto const pair : m_states) {
        delete pair.second;
    }
}