#pragma once

enum APPLICATION_PROCESS {
    PROCESS_APPSTART = 0,
    PROCESS_APPLOADSOURCE = 1,
    PROCESS_APPEND = 2,
    PROCESS_FRAMESTART = 3,
    PROCESS_FRAMEUPDATE = 4,
    PROCESS_FRAMEDRAW = 5,
    PROCESS_FRAMEEND = 6,
    PROCESS_APPRUNSOURCE = 7
};

class Runtime {
    public:
        static Runtime& getInstance() {
            static Runtime instance;
            return instance;
        }
        void process(APPLICATION_PROCESS state);
        Runtime(Runtime const&)         = delete;
        void operator=(Runtime const&)  = delete;
        void setBindings();
    private:
        Runtime() = default;
        ~Runtime() = default;

        void process_appStart();
        void process_appLoadSource();
        void process_appRunSource();
        void process_appEnd();

        void process_frameStart();
        void process_frameUpdate();
        void process_frameDraw();
        void process_frameEnd();
    private:
        bool m_isPaused {false};
};