cmake_minimum_required(VERSION 3.16)
project(installer)

set(ZLIB_USE_STATIC_LIBS ON CACHE BOOL "Use static zlib" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(ENABLE_BZIP2 OFF CACHE BOOL "Disable bzip2 if not needed" FORCE)
set(ENABLE_LZMA OFF CACHE BOOL "Disable lzma if not needed" FORCE)

add_subdirectory("extern/libzip" libzip)

add_executable(installer
        src/Main.cpp
        src/Arches.cpp
        src/Package.h
)

target_compile_definitions(installer PRIVATE ZIP_STATIC)

target_link_options(installer PRIVATE -static -static-libgcc -static-libstdc++)
target_link_libraries(installer zip bcrypt)
