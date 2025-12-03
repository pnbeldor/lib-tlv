
if(TLV_BUILD_TESTS)
    find_package(GTest REQUIRED)

    file(GLOB SOURCES CONFIG_DEPENDS "*Tests.cpp")

    add_executable(TLVTests ${SOURCES})

    target_link_libraries(TLVTests tlv::tlv GTest::gtest GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(TLVTests)
    add_test(NAME TLV_tests COMMAND TLVTests)
endif()