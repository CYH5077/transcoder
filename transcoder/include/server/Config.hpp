#pragma once

#include <thread>

#define DEFAULT_PORT 10000

namespace tr {
    class Config {
    public:
        static Config& getInstance();
        static Config instance;

    public:
        bool parse(int argc, char* argv[]);

        int getPort();
        int getThreadNum();
        std::string getTranscodeDir();
        std::string getUploadDir();

    private:
        explicit Config();
        virtual ~Config();

    private:
        int port;
        int threadNum;

        std::string uploadDir;
        std::string transcodeDir;
    };
}
