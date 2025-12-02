
if(TLV_BUILD_TESTS)
    project(TLVUtilsTests LANGUAGES CXX)

    find_package(GTest REQUIRED)

    file(GLOB SOURCES CONFIG_DEPENDS "*Tests.cpp")

    add_executable(TLVUtilsTests ${SOURCES})

    target_include_directories(TLVUtilsTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

    add_test(NAME TLVUtils_tests COMMAND TLVUtilsTests)
    target_link_libraries(TLVUtilsTests tlv::tlv tlv::utils GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(TLVUtilsTests)
endif()
