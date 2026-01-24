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

# Create game executable
add_executable(MSTour
    ${GAME_SOURCES}
    ${GAME_HEADERS}
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

message(STATUS "Game executable configured")
