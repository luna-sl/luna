cmake_minimum_required(VERSION 3.27)
project(luna)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)

find_package(OpenSSL)

set(BUILD_SHARED_LIBS OFF)
set(OPENSSL_USE_STATIC_LIBS ON)
set(ZLIB_USE_STATIC_LIBS ON)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8
)

FetchContent_MakeAvailable(cpr)
FetchContent_Declare(
    libarchive
    GIT_REPOSITORY https://github.com/libarchive/libarchive.git
    GIT_TAG 8fc0569bab522212fd593210e5a88e07918cbd5e
)

FetchContent_MakeAvailable(libarchive)

FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG 3bf268481da8208d171d8908e6491459de3651d7
)

FetchContent_MakeAvailable(cxxopts)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
message(Sources ${SOURCES})
add_executable(luna ${SOURCES})

target_link_libraries(luna PRIVATE cpr archive_static cxxopts c++ c++abi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Oz -fPIC -ffast-math -Wl,--gc-sections -mllvm -inline-threshold=0 ")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC" )

set(CMAKE_EXE_LINKER_FLAGS "-flto=full -s")
