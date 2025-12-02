# Compiler detection and configuration
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Common compiler flags for all build types
set(COMMON_FLAGS "")
set(COMMON_CXX_FLAGS "")

# MSVC compiler options
if(MSVC)
    # Remove default warning level flags
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    
    # Warning level 4 and all warnings
    list(APPEND COMMON_FLAGS "/W4")
    list(APPEND COMMON_FLAGS "/wd4251")  # Disable DLL export warning
    list(APPEND COMMON_FLAGS "/wd4275")  # Disable DLL export warning
    
    # Security and optimization flags
    list(APPEND COMMON_FLAGS "/permissive-")
    list(APPEND COMMON_FLAGS "/Zc:__cplusplus")
    list(APPEND COMMON_FLAGS "/Zc:inline")
    
    # Standard conformance
    if(CMAKE_CXX_STANDARD GREATER_EQUAL 17)
        list(APPEND COMMON_FLAGS "/Zc:__cplusplus")
    endif()
    
    # Debug flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND COMMON_FLAGS "/Od")
        list(APPEND COMMON_FLAGS "/Zi")
        list(APPEND COMMON_FLAGS "/RTC1")
    else()
        # Release flags
        list(APPEND COMMON_FLAGS "/O2")
        list(APPEND COMMON_FLAGS "/Ob2")
        list(APPEND COMMON_FLAGS "/fp:fast")
    endif()
    
    # LTO support
    if(TLV_ENABLE_LTO)
        list(APPEND COMMON_FLAGS "/GL")
        set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} /LTCG")
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
    endif()
    
    # Define Windows export symbols
    if(TLV_BUILD_SHARED)
        add_definitions(-DTLV_BUILD_DLL)
    endif()
    
# GCC/Clang compiler options
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Common warnings
    list(APPEND COMMON_FLAGS "-Wall")
    list(APPEND COMMON_FLAGS "-Wextra")
    list(APPEND COMMON_FLAGS "-Wpedantic")
    list(APPEND COMMON_FLAGS "-Wshadow")
    list(APPEND COMMON_FLAGS "-Wnon-virtual-dtor")
    list(APPEND COMMON_FLAGS "-Wold-style-cast")
    list(APPEND COMMON_FLAGS "-Wcast-align")
    list(APPEND COMMON_FLAGS "-Wunused")
    list(APPEND COMMON_FLAGS "-Woverloaded-virtual")
    #list(APPEND COMMON_FLAGS "-Wconversion")
    #list(APPEND COMMON_FLAGS "-Wsign-conversion")
    list(APPEND COMMON_FLAGS "-Wnull-dereference")
    list(APPEND COMMON_FLAGS "-Wdouble-promotion")
    list(APPEND COMMON_FLAGS "-Wformat=2")
    
    # Clang specific warnings
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        list(APPEND COMMON_FLAGS "-Wdocumentation")
        list(APPEND COMMON_FLAGS "-Wdeprecated")
        list(APPEND COMMON_FLAGS "-Weverything")
        # Disable some overly pedantic warnings
        list(APPEND COMMON_FLAGS "-Wno-c++98-compat")
        list(APPEND COMMON_FLAGS "-Wno-c++98-compat-pedantic")
        list(APPEND COMMON_FLAGS "-Wno-padded")

        list(APPEND COMMON_FLAGS "-Wno-implicit-int-conversion")
        list(APPEND COMMON_FLAGS "-Wno-sign-conversion")
        list(APPEND COMMON_FLAGS "-Wno-covered-switch-default")
    endif()
    
    # GCC specific warnings
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        list(APPEND COMMON_FLAGS "-Wduplicated-cond")
        list(APPEND COMMON_FLAGS "-Wduplicated-branches")
        list(APPEND COMMON_FLAGS "-Wlogical-op")
        list(APPEND COMMON_FLAGS "-Wuseless-cast")
        # GCC 8+ specific
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 8.0)
            list(APPEND COMMON_FLAGS "-Wclass-memaccess")
        endif()
    endif()
    
    # Standard version checks
    if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
        list(APPEND COMMON_CXX_FLAGS "-std=c++20")
    else()
        list(APPEND COMMON_CXX_FLAGS "-std=c++17")
    endif()
    
    # Debug flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND COMMON_FLAGS "-O0")
        list(APPEND COMMON_FLAGS "-g3")
        list(APPEND COMMON_FLAGS "-fno-omit-frame-pointer")
        list(APPEND COMMON_FLAGS "-fno-inline")
    else()
        # Release flags
        list(APPEND COMMON_FLAGS "-O3")
        list(APPEND COMMON_FLAGS "-march=native")
        list(APPEND COMMON_FLAGS "-ffunction-sections")
        list(APPEND COMMON_FLAGS "-fdata-sections")
        
        if(NOT MINGW AND NOT CYGWIN)
            list(APPEND COMMON_FLAGS "-D_FORTIFY_SOURCE=2")
        endif()
    endif()
    
    # LTO support
    if(TLV_ENABLE_LTO)
        list(APPEND COMMON_FLAGS "-flto")
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            list(APPEND COMMON_FLAGS "-flto=auto")
        endif()
    endif()
    
    # Position Independent Code
    if(TLV_ENABLE_PIC)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    endif()
    
    # Warnings as errors
    if(TLV_WARNINGS_AS_ERRORS)
        list(APPEND COMMON_FLAGS "-Werror")
    endif()
    
    # Sanitizers
    if(TLV_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND COMMON_FLAGS "-fsanitize=address,undefined")
        list(APPEND COMMON_FLAGS "-fno-omit-frame-pointer")
        list(APPEND COMMON_FLAGS "-fno-optimize-sibling-calls")
    endif()
endif()

# Apply common flags
string(REPLACE ";" " " COMMON_FLAGS_STR "${COMMON_FLAGS}")
string(REPLACE ";" " " COMMON_CXX_FLAGS_STR "${COMMON_CXX_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS_STR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS_STR} ${COMMON_CXX_FLAGS_STR}")

# Platform-specific linker flags
if(UNIX AND NOT APPLE)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")
    endif()
endif()

# Print summary
message(STATUS "C flags: ${CMAKE_C_FLAGS}")
message(STATUS "CXX flags: ${CMAKE_CXX_FLAGS}")

# Function to set target properties
function(set_target_compiler_options TARGET)
    target_compile_features(${TARGET} PRIVATE cxx_std_20)
    
    if(MSVC)
        target_compile_options(${TARGET} PRIVATE
            "$<$<CONFIG:Debug>:/Od;/Zi;/RTC1>"
            "$<$<CONFIG:Release>:/O2;/Ob2>"
            "$<$<CONFIG:RelWithDebInfo>:/O2;/Ob1;/Zi>"
            "$<$<CONFIG:MinSizeRel>:/O1;/Ob1>"
        )
    else()
        target_compile_options(${TARGET} PRIVATE
            "$<$<CONFIG:Debug>:-O0;-g3;-fno-omit-frame-pointer>"
            "$<$<CONFIG:Release>:-O3;-march=native>"
            "$<$<CONFIG:RelWithDebInfo>:-O2;-g>"
            "$<$<CONFIG:MinSizeRel>:-Os>"
        )
    endif()
    
    # Add sanitizers for debug builds
    if(TLV_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(NOT MSVC)
            target_compile_options(${TARGET} PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
            )
            target_link_options(${TARGET} PRIVATE
                -fsanitize=address,undefined
            )
        endif()
    endif()
endfunction()
