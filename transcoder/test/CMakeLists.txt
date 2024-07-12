set(SOURCES
        main.cpp
)

set(TRANSCODER_TEST transcoder_test)

add_executable(${TRANSCODER_TEST} ${SOURCES})

enable_testing()
add_test(AllTestsInMain main)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${TRANSCODER_TEST} PRIVATE ${TRANSCODER_LIB}
                                                 GTest::gtest
                                                 GTest::gtest_main
                                                 GTest::gmock
                                                 GTest::gmock_main)
target_include_directories(${TRANSCODER_TEST} PRIVATE ${TRANSCODER_INCLUDE_DIR})
