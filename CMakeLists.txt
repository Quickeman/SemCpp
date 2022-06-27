cmake_minimum_required(VERSION 3.10)

# Project name
project(SemCpp)

string(TOLOWER ${PROJECT_NAME} LIB_NAME)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Request C++11 standard features
set(CMAKE_CXX_STANDARD 11)


### SOURCE CODE ###

# Get a list of source files
file(GLOB SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Library of all source code
add_library(${LIB_NAME} ${SRCS})

# Add comiler flags
target_compile_options(${LIB_NAME} PRIVATE -O3 -Wall -pedantic)

# Put compiled library in bin
set_target_properties(${LIB_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Set the header to public
set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER SemCpp.hpp)

# Make PIC
set_target_properties(${LIB_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# Include the include/ directory for compilation
include_directories(${PROJECT_SOURCE_DIR}/include)

# Link the RT library
target_link_libraries(${LIB_NAME} rt)


### UNIT TESTS ###

# Enable `make test`
enable_testing()

# Get test source files
file(GLOB TEST_SRCS ${PROJECT_SOURCE_DIR}/test/*.cpp)

# Run through each test file
foreach(testSrc ${TEST_SRCS})
    # Get extension-less file name
    get_filename_component(testFileName ${testSrc} NAME_WE)
    # Make test name
    set(testName ${PROJECT_NAME}_test_${testFileName})
    # Add target
    add_executable(${testName} ${testSrc})
    # Link to relevant libraries
    target_link_libraries(${testName} ${LIB_NAME})
    # Put test executables in their own directory
    set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin)

    # Add test to execution
    add_test(
        NAME ${testName}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin
        COMMAND ${testName}
    )
endforeach(testSrc)
