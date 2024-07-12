project(transcoder)


add_library(${PROJECT_NAME})


set(TRANSCODER_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)


find_package(Drogon CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(${PROJECT_NAME} PUBLIC Drogon::Drogon
                                             Boost::system
                                             ffmpegpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
                                                    ${Boost_INCLUDE_DIRS})


add_subdirectory(src)
add_subdirectory(test)