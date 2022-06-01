#pragma once

class Clua {
    public:
        Clua();
        ~Clua();
    private:
        void setModuleBindings();
        void loadUserScripts();
        void startApplicationWindow();
};