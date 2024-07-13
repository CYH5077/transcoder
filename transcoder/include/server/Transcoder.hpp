#pragma once

#include <string>

#include "server/Config.hpp"

namespace tr {
    class Transcoder {
    public:
        static Transcoder& getInstance();
        static Transcoder instance;

    public:
        void start();
        void stop();

    private:
        explicit Transcoder();
        virtual ~Transcoder();

    private:
        void printServerInfo();
    };
};
