cmake_minimum_required(VERSION 3.10)
project(PixieSDK)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "${CMAKE_SYSTEM_NAME} is a supported environment.")
else ()
    message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is an unsupported environment!")
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/xia/${PROJECT_NAME} CACHE PATH "..." FORCE)
    endif ()
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
message(STATUS "Install Path: ${CMAKE_INSTALL_PREFIX}")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Build type, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif (NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if (${CMAKE_BUILD_TYPE} MATCHES Release)
    string(REGEX REPLACE "-O3" "-O2" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REGEX REPLACE "-O3" "-O2" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endif ()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules/;${PROJECT_SOURCE_DIR}/cmake/functions")
include(xia_configure_target)
include(CMakeDependentOption)

option(BUILD_DOCO "Tells CMake to build documentation" OFF)
option(BUILD_EXAMPLES "Builds the example programs" ON)
option(BUILD_LEGACY "Builds the legacy C API Pixie16App.a" OFF)
option(BUILD_SDK "Builds PixieSdk.a" ON)
option(BUILD_TESTS "Builds the test suites" OFF)

cmake_dependent_option(BUILD_INTEGRATION_TESTS "Builds integration tests" ON "BUILD_TESTS;BUILD_SDK" OFF)
cmake_dependent_option(BUILD_LEGACY_UNIT_TESTS "Builds unit tests" ON "BUILD_TESTS;BUILD_LEGACY" OFF)
cmake_dependent_option(BUILD_PIXIE16_API "Builds backward compatible Pixie16 SDK API Library" ON "BUILD_SDK" OFF)
cmake_dependent_option(BUILD_SYSTEM_TESTS "Enables build of system tests" ON "BUILD_TESTS;BUILD_SDK" OFF)
cmake_dependent_option(BUILD_SDK_UNIT_TESTS "Builds unit tests" ON "BUILD_TESTS;BUILD_SDK" OFF)
cmake_dependent_option(USE_USLEEP "Adds the USE_USLEEP flag to Legacy builds" "OFF" "BUILD_LEGACY" OFF)

add_subdirectory(bin)
add_subdirectory(cmake)

if (BUILD_LEGACY)
    message(WARNING "The Legacy C API will be deprecated July 31, 2023. We recommend using Pixie16Api.so instead.")
endif ()

if (BUILD_SDK OR BUILD_LEGACY)
    find_package(BroadcomAPI REQUIRED)
    IF (${CMAKE_VERSION} VERSION_LESS "3.13")
        link_directories(${PLX_LIBRARY_DIR})
    ENDIF (${CMAKE_VERSION} VERSION_LESS "3.13")

    add_subdirectory(sdk)

    if (BUILD_TESTS)
        add_subdirectory(tests)
    endif ()

    if (BUILD_EXAMPLES)
        add_subdirectory(examples)
    endif()

    add_subdirectory(externals)
endif ()

if (BUILD_DOCO)
    add_subdirectory(doc)
endif ()