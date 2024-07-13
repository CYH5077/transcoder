#include "server/Config.hpp"

#include <thread>

#include "utils/ArgParser.hpp"

namespace tr {
    Config Config::instance;

    Config& Config::getInstance() {
        return Config::instance;
    }

    Config::Config() : port(DEFAULT_PORT), threadNum(std::thread::hardware_concurrency()) {}

    Config::~Config() {}

    bool Config::parse(int argc, char* argv[]) {
        tr::ArgParser::setFlagInt("-p", "port", &Config::instance.port);
        tr::ArgParser::setFlagInt("-t", "thread number", &Config::instance.threadNum);

        if (tr::ArgParser::parse(argc, argv) == false) {
            return false;
        }

        if (Config::instance.port == 0) {
            Config::instance.port = DEFAULT_PORT;
        }

        if (Config::instance.threadNum == 0) {
            Config::instance.threadNum = std::thread::hardware_concurrency();
        }

        return true;
    }

    int Config::getPort() {
        return Config::instance.port;
    }

    int Config::getThreadNum() {
        return Config::instance.threadNum;
    }
}  // namespace tr
