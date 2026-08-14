cmake_minimum_required(VERSION 3.16)

project(installer)

add_subdirectory("extern/libzip" libzip)

add_executable(installer
        src/Main.cpp
        src/Package.h
)

target_link_libraries(installer zip)
