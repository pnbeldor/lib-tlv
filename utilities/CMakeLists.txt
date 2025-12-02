
file(GLOB_RECURSE UTILS_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

set(TLV_UTILS_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/HEXDump.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TLVUtils.h
)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# Shared library target
if(TLV_BUILD_SHARED)
    add_library(tlv_utils_shared SHARED ${UTILS_SOURCES})
    target_include_directories(tlv_utils_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

    # Set properties
    set_target_properties(tlv_utils_shared PROPERTIES
        OUTPUT_NAME tlv_utils
        VERSION ${TLV_VERSION}
        SOVERSION ${TLV_VERSION_MAJOR}
        POSITION_INDEPENDENT_CODE ${TLV_ENABLE_PIC}
    )
    
    # Set compiler options
    if(COMMAND set_target_compiler_options)
        set_target_compiler_options(tlv_utils_shared)
    endif()

        # Export symbols
    if(WIN32)
        target_compile_definitions(tlv_utils_shared PRIVATE TLV_EXPORT)
    endif()

    # Install target
    install(TARGETS tlv_utils_shared
        EXPORT tlv-targets
        RUNTIME DESTINATION ${TLV_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${TLV_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${TLV_INSTALL_LIB_DIR}
    )
    
    # Alias target
    add_library(tlv::utils ALIAS tlv_utils_shared)

endif()

# Static library target
if(TLV_BUILD_STATIC)
    add_library(tlv_utils_static STATIC)
    target_sources(tlv_utils_static PRIVATE ${UTILS_SOURCES})

    target_include_directories(tlv_utils_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

    target_link_libraries(tlv_utils_static PUBLIC tlv_static)

        # Set properties
    set_target_properties(tlv_utils_static PROPERTIES
        OUTPUT_NAME tlv_utils-static
        POSITION_INDEPENDENT_CODE ${TLV_ENABLE_PIC}
    )

    # Set compiler options
    if(COMMAND set_target_compiler_options)
        set_target_compiler_options(tlv_utils_static)
    endif()

    if(WIN32 AND TLV_BUILD_SHARED)
        target_compile_definitions(tlv_utils_static PRIVATE TLV_STATIC_DEFINE)
    endif()

    # Alias target for static only builds
    if(NOT TLV_BUILD_SHARED)
        add_library(tlv::utils ALIAS tlv_utils_static)
    endif()

endif()

if(TLV_BUILD_TESTS)
    add_subdirectory(tests)
endif()
