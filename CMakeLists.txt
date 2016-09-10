# Basic CMake project
cmake_minimum_required(VERSION 2.8.2)

# Output compilation database for clangtidy
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Set default build type to Release
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Include the current directory in every build
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
# Name the project
project(primes CXX)

# Enable C++17 features on gcc/clang
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Clang)")
    set(CMAKE_CXX_FLAGS "-fcolor-diagnostics -Weverything -Wpedantic -Werror -std=c++1z")
    # Turn off select warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat -Wno-padded")
    # TODO Remove on C++17 compatibility
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-implicit-fallthrough")
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GCC)")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++1z")
    set(CMAKE_CXX_FLAGS_DEBUG "-Og")
endif()

# Add pthread support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Locate Boost libraries: unit_test_framework
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Add primes to a lib
add_library(primes primes.cpp primes.h)

# Build executable from sources and headers
add_executable(unittest primes test/unittest.cpp)

# Link to Boost
target_link_libraries(unittest primes ${Boost_LIBRARIES})

# Turn off warnings on unittest file due to Boost
set_source_files_properties(test/unittest.cpp PROPERTIES COMPILE_FLAGS "-w")

# Add this as a test
enable_testing()
add_test(unittest unittest)
add_custom_command(TARGET unittest
                   POST_BUILD
                   COMMAND unittest
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Add benchmark executable
add_executable(benchmark test/benchmark.cpp)
target_link_libraries(benchmark primes)

