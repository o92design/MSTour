# Tests CMakeLists.txt

# Collect test source files
file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# Create test executable
add_executable(MSTour_tests
    ${TEST_SOURCES}
)

# Include directories
target_include_directories(MSTour_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(MSTour_tests
    PRIVATE
        engine
        GTest::gtest_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(MSTour_tests)

message(STATUS "Tests configured")
