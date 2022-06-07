#pragma once

class Clua {
    public:
        Clua();
        ~Clua();
    private:
        void bindModules();
        void startWindow();
};