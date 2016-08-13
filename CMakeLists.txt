# Basic CMake project
cmake_minimum_required(VERSION 2.8.2)

# Set default build type to Release
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.gtest googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
 
# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
 
# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)


# Include the current directory in every build
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(proj primes)

# Name the project
project(primes CXX)

# Enable C++14 features on gcc/clang
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)")
    set(CMAKE_CXX_FLAGS "-std=c++14")
endif()

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")

enable_testing()

# Build executable from sources and headers
add_executable(unittest primes.cpp primes.h test/unittest.cpp)

# Link to gtest
target_link_libraries(unittest gtest_main)

# Add this as a test
add_test(NAME unittest COMMAND unittest)

# Add benchmark executable
add_executable(benchmark primes.cpp primes.h test/benchmark.cpp)

