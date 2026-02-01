# Tests CMakeLists.txt

# Collect test source files
file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# Collect game source files (exclude main.c to avoid duplicate main())
file(GLOB_RECURSE GAME_SOURCES
    "${CMAKE_SOURCE_DIR}/game/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/game/src/*.c"
)

# Filter out main.c from game sources
list(FILTER GAME_SOURCES EXCLUDE REGEX ".*main\\.c$")

# Create test executable
add_executable(MSTour_tests
    ${TEST_SOURCES}
    ${GAME_SOURCES}
)

# Include directories
target_include_directories(MSTour_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/game/include
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
