#pragma once

class Clua {
    public:
        Clua();
        ~Clua() = default;
    private:
        void bindModules();
        void startWindow();
};