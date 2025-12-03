# vim: et ts=4 sts=4 sw=4 tw=0

cmake_minimum_required(VERSION 4.1.0)
project(tlv-library
    VERSION 1.0.0
    LANGUAGES C CXX
    DESCRIPTION "Type-Length-Value Library"
)

# Set project policies
cmake_policy(SET CMP0077 NEW)  # option() honors normal variables


# Version information
set(TLV_VERSION_MAJOR 1)
set(TLV_VERSION_MINOR 0)
set(TLV_VERSION_PATCH 0)
set(TLV_VERSION "${TLV_VERSION_MAJOR}.${TLV_VERSION_MINOR}.${TLV_VERSION_PATCH}")

# Build the library with C++20 standard support, independent from other including
# software which may use a different CXX_STANDARD or CMAKE_CXX_STANDARD.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "${PROJECT_NAME} Version: ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")


include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
  #GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Default build type (Release if not specified)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Include compiler detection and options
include(cmake/CompilerOptions.cmake)
include(cmake/PlatformSpecific.cmake)

# Library options
option(TLV_BUILD_SHARED "Build shared library" ON)
option(TLV_BUILD_STATIC "Build static library" ON)
option(TLV_BUILD_EXAMPLES "Build example programs" ON)
option(TLV_BUILD_TESTS "Build tests" ON)
option(TLV_ENABLE_LTO "Enable Link Time Optimization" OFF)
option(TLV_ENABLE_PIC "Enable Position Independent Code" ON)
option(TLV_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(TLV_ENABLE_SANITIZERS "Enable sanitizers (Address, Undefined)" OFF)

# Installation options
set(TLV_INSTALL_LIB_DIR lib CACHE STRING "Library installation directory")
set(TLV_INSTALL_BIN_DIR bin CACHE STRING "Binary installation directory")
set(TLV_INSTALL_INCLUDE_DIR lib/include CACHE STRING "Header installation directory")


include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Package export
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/tlv-config-version.cmake
    VERSION ${TLV_VERSION}
    COMPATIBILITY AnyNewerVersion
)

file(GLOB_RECURSE TLV_SOURCES  "src/*.cpp" "src/utilities/*.cpp")

# Shared library target
if(TLV_BUILD_SHARED)
    add_library(tlv_shared SHARED ${TLV_SOURCES})

    target_include_directories(tlv_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
    include_directories(tlv_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/include")
    
    # Set properties
    set_target_properties(tlv_shared PROPERTIES
        OUTPUT_NAME tlv
        VERSION ${TLV_VERSION}
        SOVERSION ${TLV_VERSION_MAJOR}
        POSITION_INDEPENDENT_CODE ${TLV_ENABLE_PIC}
    )
    
    # Set compiler options
    if(COMMAND set_target_compiler_options)
        set_target_compiler_options(tlv_shared)
    endif()
    
    # Export symbols
    if(WIN32)
        target_compile_definitions(tlv_shared PRIVATE TLV_EXPORT)
    endif()
    
    # Install target
    install(TARGETS tlv_shared
        EXPORT tlv-targets
        RUNTIME DESTINATION ${TLV_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${TLV_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${TLV_INSTALL_LIB_DIR}
    )
    
    # Alias target
    add_library(tlv::tlv ALIAS tlv_shared)
endif()

# Static library target
if(TLV_BUILD_STATIC)
    add_library(tlv_static STATIC ${TLV_SOURCES})
    
    target_include_directories(tlv_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
    include_directories(tlv_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/include")
    
    # Set properties
    set_target_properties(tlv_static PROPERTIES
        OUTPUT_NAME tlv
        VERSION ${TLV_VERSION}
        SOVERSION ${TLV_VERSION_MAJOR}
        POSITION_INDEPENDENT_CODE ${TLV_ENABLE_PIC}
    )
    
    # Set compiler options
    if(COMMAND set_target_compiler_options)
        set_target_compiler_options(tlv_static)
    endif()
    
    if(WIN32 AND TLV_BUILD_SHARED)
        target_compile_definitions(tlv_static PRIVATE TLV_STATIC_DEFINE)
    endif()
    
    # Install target
    install(TARGETS tlv_static
        EXPORT tlv-targets
        ARCHIVE DESTINATION ${TLV_INSTALL_LIB_DIR}
    )
    
    # Alias target for static only builds
    if(NOT TLV_BUILD_SHARED)
        add_library(tlv::tlv ALIAS tlv_static)
    endif()
endif()

if(TLV_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(TLV_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

# Export targets for find_package
export(EXPORT tlv-targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/tlv-targets.cmake"
    NAMESPACE tlv::
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tlv-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/tlv-config.cmake
    INSTALL_DESTINATION lib/cmake/tlv
)
