# Game Executable CMakeLists.txt

# Collect game source files
file(GLOB_RECURSE GAME_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE GAME_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# Get cJSON source (it's not header-only)
set(CJSON_SOURCE ${cjson_SOURCE_DIR}/cJSON.c)

# Create game executable
add_executable(MSTour
    ${GAME_SOURCES}
    ${GAME_HEADERS}
    ${CJSON_SOURCE}
)

# Include directories
target_include_directories(MSTour
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link engine library
target_link_libraries(MSTour
    PRIVATE
        engine
        cjson_header
)

# Set output directory for the executable
set_target_properties(MSTour PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Platform-specific settings
if(WIN32)
    # For Windows, we might want a console window for debugging
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set_target_properties(MSTour PROPERTIES
            WIN32_EXECUTABLE FALSE
        )
    else()
        set_target_properties(MSTour PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    endif()
endif()

# Copy assets folder to build directory
add_custom_command(TARGET MSTour POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:MSTour>/assets"
    COMMENT "Copying assets to build directory"
)

# Copy config.ini to build directory
add_custom_command(TARGET MSTour POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/config.ini"
        "$<TARGET_FILE_DIR:MSTour>/config.ini"
    COMMENT "Copying config.ini to build directory"
)

message(STATUS "Game executable configured")
