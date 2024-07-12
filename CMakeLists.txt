cmake_minimum_required(VERSION 3.6)
project(transcoder_server)


find_package(Git QUIET)
if(NOT GIT_FOUND)
  message(FATAL_ERROR "Not installed git")
endif()


function(update_submodule SUBMODULE_PATH)
  message(STATUS "${SUBMODULE_PATH} submodule update.....")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote ${SUBMODULE_PATH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMOD_RESULT
  )
  if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "${SUBMODULE_PATH} submodule update failed")
  endif()
  message(STATUS "${SUBMODULE_PATH} submodule update success")
endfunction()


update_submodule("")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (WIN32 AND MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
elseif (LINUX)
    set(CMAKE_CXX_FLAGS -Wall)
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif ()


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE transcoder)


add_subdirectory(submodule)
add_subdirectory(transcoder)

