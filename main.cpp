#include "server/Transcoder.hpp"

int main(int argc, char* argv[]) {
    if (tr::Config::parse(argc, argv) == false) {
        return -1;
    }

    tr::Transcoder::getInstance().start();

    return 0;
}
