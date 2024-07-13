#include "gtest/gtest.h"
#include "utils/ArgParser.hpp"

TEST(ArgParser_TEST, ArgParser_TEST) {
    double doubleValue = 0.0;
    float floatValue = 0.0f;
    std::string stringValue;
    int intValue = 0;
    bool boolValue = false;

    tr::ArgParser::setFlagDouble("-d", "double value", &doubleValue);
    tr::ArgParser::setFlagFloat("-f", "float value", &floatValue);
    tr::ArgParser::setFlagString("-s", "string value", &stringValue);
    tr::ArgParser::setFlagInt("-i", "int value", &intValue);
    tr::ArgParser::setFlagBool("-b", "bool value", &boolValue);

    char* argv[] = {(char*)"test.exe", (char*)"-d", (char*)"3.14", (char*)"-f", (char*)"3.14", (char*)"-s",
                    (char*)"hello",    (char*)"-i", (char*)"100",  (char*)"-b", (char*)"true"};

    ASSERT_TRUE(tr::ArgParser::parse(10, argv));

    EXPECT_EQ(doubleValue, 3.14);
    EXPECT_EQ(floatValue, 3.14f);
    EXPECT_EQ(stringValue, "hello");
    EXPECT_EQ(intValue, 100);
    EXPECT_EQ(boolValue, true);
}

TEST(ArgParser_TEST, HELP) {
    char* argv[] = {(char*)"test.exe", (char*)"-h"};
    tr::ArgParser::parse(2, argv);
}
