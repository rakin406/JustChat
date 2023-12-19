cmake_minimum_required(VERSION 3.26)
project(JustChat CXX)

# Explicitly opt in to modern CMake behaviors to avoid warnings with recent
# versions of CMake.
cmake_policy(SET CMP0063 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Download CPM.cmake
file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
    EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# ---- Dependencies ----

find_package(Threads REQUIRED)

CPMAddPackage("gh:chriskohlhoff/asio#asio-1-29-0@1.29.0")

# ASIO doesn't use CMake, we have to configure it manually. Extra notes for using on Windows:
#
# 1) If _WIN32_WINNT is not set, ASIO assumes _WIN32_WINNT=0x0501, i.e. Windows XP target, which is
# definitely not the platform which most users target.
#
# 2) WIN32_LEAN_AND_MEAN is defined to make Winsock2 work.
if(asio_ADDED)
    add_library(asio INTERFACE)

    target_include_directories(asio SYSTEM INTERFACE ${asio_SOURCE_DIR}/asio/include)

    target_compile_definitions(asio INTERFACE ASIO_STANDALONE ASIO_NO_DEPRECATED)

    target_link_libraries(asio INTERFACE Threads::Threads)

    if(WIN32)
        # macro see @ https://stackoverflow.com/a/40217291/1746503
        macro(get_win32_winnt version)
            if(CMAKE_SYSTEM_VERSION)
                set(ver ${CMAKE_SYSTEM_VERSION})
                string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
                string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
                # Check for Windows 10, b/c we'll need to convert to hex 'A'.
                if("${verMajor}" MATCHES "10")
                    set(verMajor "A")
                    string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
                endif("${verMajor}" MATCHES "10")
                # Remove all remaining '.' characters.
                string(REPLACE "." "" ver ${ver})
                # Prepend each digit with a zero.
                string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
                set(${version} "0x${ver}")
            endif()
        endmacro()

        if(NOT DEFINED _WIN32_WINNT)
            get_win32_winnt(ver)
            set(_WIN32_WINNT ${ver})
        endif()

        message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

        target_compile_definitions(asio INTERFACE _WIN32_WINNT=${_WIN32_WINNT} WIN32_LEAN_AND_MEAN)
    endif()
endif()

# ---- Executable ----

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/server.cpp
    include/server.hpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE asio)

# Set compiler warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /Wall /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Werror
        $<$<COMPILE_LANGUAGE:CXX>:-Wold-style-cast>
        -Wno-unused-parameter
    )
endif()
