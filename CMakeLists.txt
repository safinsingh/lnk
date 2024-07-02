cmake_minimum_required(VERSION 3.10)

project(lnk)

set(CMAKE_C_STANDARD 99)

set(CMAKE_BUILD_TYPE Debug)

include_directories(
	inc/
	vendor/maps/
)

set(SOURCE_FILES

	src/config.c
    src/error.c
    src/main.c
    src/net.c
    src/squeue.c
    src/threading.c

	vendor/maps/hash_map.c
	vendor/maps/djb2a.c
)

add_executable(lnk ${SOURCE_FILES})

target_compile_options(lnk PRIVATE -g)
find_package(Threads REQUIRED)
target_link_libraries(lnk PRIVATE Threads::Threads m)
