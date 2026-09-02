cmake_minimum_required(VERSION 3.16)

project(thread_pool_application LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_library(thread_pool
    src/ThreadPool.cpp
)

target_include_directories(thread_pool
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(thread_pool
    PUBLIC
        Threads::Threads
)

add_executable(thread_pool_app
    src/main.cpp
)

target_link_libraries(thread_pool_app
    PRIVATE
        thread_pool
)

enable_testing()

add_executable(thread_pool_test
    tests/thread_pool_test.cpp
)

target_link_libraries(thread_pool_test
    PRIVATE
        thread_pool
)

add_test(NAME thread_pool_test COMMAND thread_pool_test)
