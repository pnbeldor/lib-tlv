# Platform detection
if(WIN32)
    set(TLV_PLATFORM_WINDOWS 1)
    set(TLV_PLATFORM_NAME "Windows")
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    
    # For MSVC, set secure CRT
    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    endif()
    
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(TLV_PLATFORM_LINUX 1)
    set(TLV_PLATFORM_UNIX 1)
    set(TLV_PLATFORM_NAME "Linux")
    add_definitions(-D_GNU_SOURCE)
    add_definitions(-D_POSIX_C_SOURCE=200809L)
    
elseif(APPLE)
    set(TLV_PLATFORM_MACOS 1)
    set(TLV_PLATFORM_UNIX 1)
    set(TLV_PLATFORM_NAME "macOS")
    add_definitions(-D_DARWIN_C_SOURCE)
    
    # macOS version target
    if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version")
    endif()
endif()

# Compiler detection
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(TLV_COMPILER_MSVC 1)
    set(TLV_COMPILER_NAME "MSVC")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(TLV_COMPILER_CLANG 1)
    set(TLV_COMPILER_NAME "Clang")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(TLV_COMPILER_GNU 1)
    set(TLV_COMPILER_NAME "GCC")
endif()

# Build type detection
if(TLV_BUILD_SHARED)
    set(TLV_SHARED 1)
    if(WIN32)
        set(TLV_BUILD_DLL 1)
    endif()
elseif(TLV_BUILD_STATIC)
    set(TLV_STATIC 1)
endif()

# Create build directory for config.h
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)

# Configure config.h
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.in
    ${CMAKE_BINARY_DIR}/include/onfig.h
    @ONLY
)

# Platform-specific linker flags
if(UNIX AND NOT APPLE)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")
    endif()
endif()

# Print configuration summary
message(STATUS "Platform: ${TLV_PLATFORM_NAME}")
message(STATUS "Compiler: ${TLV_COMPILER_NAME} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Shared library: ${TLV_BUILD_SHARED}")
message(STATUS "Static library: ${TLV_BUILD_STATIC}")
