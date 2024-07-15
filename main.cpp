#include "server/Transcoder.hpp"

#include "ffmpegpp.hpp"

int main(int argc, char* argv[]) {
    if (tr::Config::parse(argc, argv) == false) {
        return -1;
    }

    ff::ffmpegLogOff();

    tr::Transcoder::getInstance().start();

    return 0;
}
