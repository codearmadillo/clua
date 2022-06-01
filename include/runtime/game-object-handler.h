#pragma once

class GameObjectHandler {
    public:
        static GameObjectHandler& getInstance() {
            static GameObjectHandler instance;
            return instance;
        }
        GameObjectHandler(GameObjectHandler const&) = delete;
        void operator=(GameObjectHandler const&) = delete;
        void setBindings();
    private:
        GameObjectHandler() = default;
};