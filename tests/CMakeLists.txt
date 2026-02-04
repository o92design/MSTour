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

# Filter out the game entry point (exact path to avoid removing other main.c files)
set(GAME_MAIN_SOURCE "${CMAKE_SOURCE_DIR}/game/src/main.c")
list(REMOVE_ITEM GAME_SOURCES "${GAME_MAIN_SOURCE}")

# Get cJSON source
set(CJSON_SOURCE ${cjson_SOURCE_DIR}/cJSON.c)

# Create test executable
add_executable(MSTour_tests
    ${TEST_SOURCES}
    ${GAME_SOURCES}
    ${CJSON_SOURCE}
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
        cjson_header
        GTest::gtest_main
)

# Copy config.ini to test directory for physics tuning tests
# Copy to both the executable directory and the CTest working directory
add_custom_command(TARGET MSTour_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/config.ini"
        "$<TARGET_FILE_DIR:MSTour_tests>/config.ini"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/config.ini"
        "${CMAKE_CURRENT_BINARY_DIR}/config.ini"
    COMMENT "Copying config.ini to test directories"
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(MSTour_tests)

message(STATUS "Tests configured")
