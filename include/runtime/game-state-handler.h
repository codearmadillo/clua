#pragma once

class GameStateHandler {
    public:
        static GameStateHandler& getInstance() {
            static GameStateHandler instance;
            return instance;
        }
        GameStateHandler(GameStateHandler const&) = delete;
        void operator=(GameStateHandler const&) = delete;
        void setBindings();
    private:
        GameStateHandler() = default;
};