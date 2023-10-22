# CMAKE VERSION
cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "async" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

# SUBPROJECTS
add_subdirectory(async)
add_subdirectory(unit-tests)
add_subdirectory(demo)

# Testing
enable_testing()

# Package
include(GNUInstallDirs)
set_target_properties(async PROPERTIES PUBLIC_HEADER async/async.h)
install (
        TARGETS
        async
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install (
        TARGETS
        async_demo
        RUNTIME DESTINATION bin
)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)