#pragma once

#include <string>

#include "utils/Config.hpp"

namespace tr {
    class Transcoder {
    public:
        static Transcoder& getInstance();
        static Transcoder instance;

    public:
        void start();

    private:
        explicit Transcoder();
        virtual ~Transcoder();

    private:
        void printServerInfo();
    };
};
