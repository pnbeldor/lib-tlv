
if(TLV_BUILD_TESTS)
    #project(TLVTests LANGUAGES CXX)
    find_package(GTest REQUIRED)

    file(GLOB SOURCES CONFIG_DEPENDS "*Tests.cpp")

    add_executable(TLVTests ${SOURCES})

    add_test(NAME TLV_tests COMMAND TLVTests)
    target_link_libraries(TLVTests tlv::tlv GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(TLVTests)
endif()