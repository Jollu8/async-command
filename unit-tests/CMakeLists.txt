# CMAKE VERSION
cmake_minimum_required(VERSION 3.23)

# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "async" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
set(SOURCES_DIR "../async")

# Boost
find_package(Boost COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${SOURCES_DIR})

# SOURCES
set (
        SOURCES
        ${SOURCES_DIR}/handler.cpp
        ${SOURCES_DIR}/state.cpp
        ${SOURCES_DIR}/logger.cpp
        ${SOURCES_DIR}/async.cpp
        gtest_handler.cpp
        gtest_async.cpp
)

# HEADERS
set (
        HEADERS
        ${SOURCES_DIR}/assignments.h
        ${SOURCES_DIR}/handler.h
        ${SOURCES_DIR}/state.h
        ${SOURCES_DIR}/logger.h
        ${SOURCES_DIR}/async.h
)

# EXECUTABLE
add_executable(gtest_handler ${SOURCES} ${HEADERS})
add_executable(gtest_async ${SOURCES} ${HEADERS})

# PROPERTIES
set_target_properties (
        gtest_handler
        gtest_async
        PROPERTIES
        CMAKE_CXX_STANDARD 20
        CMAKE_CXX_STANDARD_REQUIRED ON
)

# INCLUDEPATH
target_include_directories(gtest_handler PRIVATE ${SOURCES_DIR})
target_include_directories(gtest_async PRIVATE ${SOURCES_DIR})

# Linking
find_package (Threads)
target_link_libraries (
        gtest_handler
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_FILESYSTEM_LIBRARY}
)
target_link_libraries (
        gtest_async
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_FILESYSTEM_LIBRARY}
)

# Compiler options
if (MSVC)
    target_compile_options(gtest_handler PRIVATE
            /W4
    )
    target_compile_options(gtest_async PRIVATE
            /W4
    )
else ()
    target_compile_options(gtest_handler PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(gtest_async PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
endif()

## UNIT-tests
# GoogleTest package
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Tests linking
gtest_discover_tests(gtest_handler)
target_link_libraries(
        gtest_handler
        gtest_main
)
gtest_discover_tests(gtest_async)
target_link_libraries(
        gtest_async
        gtest_main
)

# Testing
enable_testing()
add_test(gtest_handler gtest_handler)
add_test(gtest_async gtest_async)