# CMAKE VERSION
cmake_minimum_required(VERSION 3.23)

# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "async" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VESRION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../async")

# EXECUTABLE
add_executable(async_demo main.cpp)

# PROPERTIES
set_target_properties (
        async_demo
        PROPERTIES
        CMAKE_CXX_STANDARD 17
        CMAKE_CXX_STANDARD_REQUIRED ON
)

# INCLUDEPATH
include_directories(${PROJECT_SOURCE_DIR}/../async/)
target_include_directories(
        async_demo
        PUBLIC
        ${PROJECT_SOURCE_DIR}/../async/
)

# Linking
find_package (Threads)
target_link_libraries (async_demo async ${CMAKE_THREAD_LIBS_INIT})

# Compiler options
if (MSVC)
    target_compile_options(async_demo PRIVATE
            /W4
    )
else ()
    target_compile_options(async_demo PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
endif()