cmake_minimum_required(VERSION 3.26)
project(JustChat CXX)

# Explicitly opt in to modern CMake behaviors to avoid warnings with recent
# versions of CMake.
cmake_policy(SET CMP0063 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#include(FetchContent)
#set(FETCHCONTENT_QUIET FALSE)
#
#FetchContent_Declare(
#    entt
#    GIT_REPOSITORY https://github.com/skypjack/entt.git
#    GIT_TAG        v3.12.2
#    GIT_PROGRESS   TRUE
#    SYSTEM
#)

#FetchContent_MakeAvailable(entt)

#add_executable(
#    ${PROJECT_NAME}
#    src/Main.cpp
#)
#target_include_directories(${PROJECT_NAME} PRIVATE include)

# Set compiler warnings
#if (MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /Wall /WX)
#else()
#    target_compile_options(${PROJECT_NAME} PRIVATE
#        -Wall
#        -Wextra
#        -Werror
#        $<$<COMPILE_LANGUAGE:CXX>:-Wold-style-cast>
#        -Wno-unused-parameter
#    )
#endif()

#target_link_libraries(${PROJECT_NAME} PRIVATE )