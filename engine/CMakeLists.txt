# Engine Library CMakeLists.txt

# Collect engine source files
file(GLOB_RECURSE ENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE ENGINE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# Create engine library
add_library(engine STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

# Include directories
target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link Raylib
target_link_libraries(engine
    PUBLIC
        raylib
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(engine PUBLIC PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(engine PUBLIC PLATFORM_LINUX)
endif()

message(STATUS "Engine library configured")
