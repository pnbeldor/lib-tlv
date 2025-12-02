if(TLV_BUILD_EXAMPLES)
    # Basic example
    add_executable(tlv_example_basic basic_example.cpp)
    target_link_libraries(tlv_example_basic 
        $<$<BOOL:${TLV_BUILD_SHARED}>:tlv_shared>
        $<$<NOT:$<BOOL:${TLV_BUILD_SHARED}>>:tlv_static>
        tlv::tlv
        tlv::utils
    )
    set_target_compiler_options(tlv_example_basic)
    
    # Example 2: Advanced TLV usage
    add_executable(tlv_example_advanced advanced_example.cpp)
    target_link_libraries(tlv_example_advanced 
        $<$<BOOL:${TLV_BUILD_SHARED}>:tlv_shared>
        $<$<NOT:$<BOOL:${TLV_BUILD_SHARED}>>:tlv_static>
        tlv::tlv
        tlv::utils
    )
    set_target_compiler_options(tlv_example_advanced)
    
    # Example 3: Array TLV usage
    add_executable(tlv_example_array array_example.cpp)
    target_link_libraries(tlv_example_array 
        $<$<BOOL:${TLV_BUILD_SHARED}>:tlv_shared>
        $<$<NOT:$<BOOL:${TLV_BUILD_SHARED}>>:tlv_static>
        tlv::tlv
        tlv::utils
    )
    set_target_compiler_options(tlv_example_array)

    # Example 4: Parsing usage
    add_executable(tlv_example_parsing parsing_example.cpp)
    target_link_libraries(tlv_example_parsing 
        $<$<BOOL:${TLV_BUILD_SHARED}>:tlv_shared>
        $<$<NOT:$<BOOL:${TLV_BUILD_SHARED}>>:tlv_static>
        tlv::tlv
        tlv::utils
    )
    set_target_compiler_options(tlv_example_parsing)

        # Example 4: Parsing usage
    add_executable(tlv_example_dump dump_example.cpp)
    target_link_libraries(tlv_example_dump
        $<$<BOOL:${TLV_BUILD_SHARED}>:tlv_shared>
        $<$<NOT:$<BOOL:${TLV_BUILD_SHARED}>>:tlv_static>
        tlv::tlv
        tlv::utils
    )
    set_target_compiler_options(tlv_example_dump)
    
    # Install examples (optional)
    if(INSTALL_EXAMPLES)
        install(TARGETS
        tlv_example_basic
        tlv_example_advanced
        tlv_example_parsing
        tlv_example_array
        tlv_example_dump
        DESTINATION bin/examples
        )
    endif()
endif()
