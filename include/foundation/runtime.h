#pragma once

class Runtime {
    public:
        static Runtime& getInstance() {
            static Runtime instance;
            return instance;
        }
        void onBeforeWindowStart();
        void onWindowUpdate();
        Runtime(Runtime const&)         = delete;
        void operator=(Runtime const&)  = delete;
        void setBindings();
    private:
        Runtime();
        ~Runtime();
};