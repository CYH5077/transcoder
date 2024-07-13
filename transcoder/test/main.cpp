#include "gtest/gtest.h"

#include "server/Transcoder.hpp"

class TranscoderServer : public ::testing::Environment {
public:
    void SetUp() override {
        std::thread([] { tr::Transcoder::getInstance().start(); }).detach();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    void TearDown() override {
        tr::Transcoder::getInstance().stop();
    }
};


int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    ::testing::AddGlobalTestEnvironment(new TranscoderServer);

    return RUN_ALL_TESTS();
}
