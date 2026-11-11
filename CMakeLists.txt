cmake_minimum_required(VERSION 3.20)

project(BlackScholesEngine
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Black-Scholes library
add_library(blackscholes
    src/black_scholes.cpp
    src/greeks.cpp
    src/binomial.cpp
)

target_include_directories(blackscholes
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Main application
add_executable(BlackScholesEngine
    src/main.cpp
)

target_link_libraries(BlackScholesEngine
    PRIVATE blackscholes
)

# Enable testing
include(CTest)

if(BUILD_TESTING)

    # Download GoogleTest
    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    # Build test executable
    add_executable(BlackScholesTests
        tests/test_black_scholes.cpp
    )

    target_link_libraries(BlackScholesTests
        PRIVATE
            blackscholes
            GTest::gtest_main
    )

    # Discover tests
    include(GoogleTest)

    gtest_discover_tests(BlackScholesTests)

endif()