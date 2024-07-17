#include "ffmpegpp.hpp"
#include "gtest/gtest.h"
#include "server/Transcoder.hpp"

class TranscoderServer : public ::testing::Environment {
public:
    void SetUp() override {
        this->thread = std::make_shared<std::thread>([] { tr::Transcoder::getInstance().start(); });
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    void TearDown() override {
        tr::Transcoder::getInstance().stop();
        if (this->thread->joinable()) {
            this->thread->join();
        }
    }

private:
    std::shared_ptr<std::thread> thread;
};

int main(int argc, char* argv[]) {
    ff::ffmpegLogOff();

    ::testing::InitGoogleTest(&argc, argv);

    ::testing::AddGlobalTestEnvironment(new TranscoderServer);

    return RUN_ALL_TESTS();
}
