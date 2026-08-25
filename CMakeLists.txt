#-----------------------------------------------------------------------------------------------------------------------
# Information
#-----------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 4.3.0)
project("btree"
        VERSION 0.1.0
        DESCRIPTION ""
        LANGUAGES CXX
)

# CMake Information
cmake_file_api(
        QUERY
        API_VERSION 1
        CODEMODEL 2.3
)

#-----------------------------------------------------------------------------------------------------------------------
# Settings
#-----------------------------------------------------------------------------------------------------------------------
if(PROJECT_IS_TOP_LEVEL)
    # C++ Settings
    set(CMAKE_CXX_STANDARD 26)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # Ensure the current source directory contains the .clang directory
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.clang)

    # Create a symlink to compile_commands.json file
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/.clang/compile_commands.json
    )

    # Project Settings
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    # Project Options
    option(BUILD_TESTS "Build the project's test suite" OFF)

    # Set default build type if not specified
    if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    endif()
endif()

#-----------------------------------------------------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------------------------------------------------
if(BUILD_TESTS)
    find_package(GTest REQUIRED)
endif()

include(GNUInstallDirs)

#-----------------------------------------------------------------------------------------------------------------------
# Installation
#-----------------------------------------------------------------------------------------------------------------------
if(NOT CMAKE_SKIP_INSTALL_RULES)
    # Install headers
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
            TYPE INCLUDE)
endif()

#-----------------------------------------------------------------------------------------------------------------------
# Target: btree_headers
# Alias: collections::btree_headers
# Type: Interface Library
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_library(btree_headers INTERFACE)

# Target alias
add_library(collections::btree_headers ALIAS btree_headers)

# Target headers
target_include_directories(btree_headers
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#-----------------------------------------------------------------------------------------------------------------------
# Target: btree
# Alias: collections::btree
# Type: Interface Library
# Dependencies: collections::btree_headers
#-----------------------------------------------------------------------------------------------------------------------
# Target definition
add_library(btree INTERFACE)

# Target alias
add_library(collections::btree ALIAS btree)

# Target dependencies
target_link_libraries(btree
    INTERFACE
        collections::btree_headers
)

# Target properties
set_target_properties(btree PROPERTIES
        OUTPUT_NAME btree
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)

if(NOT CMAKE_SKIP_INSTALL_RULES)
    # Install btree
    install(TARGETS btree
            RUNTIME COMPONENT btree
            LIBRARY COMPONENT btree
            NAMELINK_COMPONENT btree
            ARCHIVE COMPONENT btree
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

#-----------------------------------------------------------------------------------------------------------------------
# Subdirectories
#-----------------------------------------------------------------------------------------------------------------------
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
