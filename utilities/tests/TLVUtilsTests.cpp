
#include <thread>
#include <chrono>

#include "gtest/gtest.h"

#include "TLVUtils.h"

#include "AdvancedTLV.h"

class TLVUtilsTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        
    }

    void TearDown() override
    {

    }
};

TEST_F(TLVUtilsTestFixture, BasicHEXDump)
{
    EXPECT_EQ(true, true);
    auto tlv = AdvancedTLV::CreateInteger(42);
    
    TLVUtils utils;
    std::string dump = utils.HEXDumpAdvancedTLV(*tlv);
    //std::cout << "Dump: <" << dump << ">\n";

    //std::this_thread::sleep_for(std::chrono::seconds(5));
    EXPECT_NE(dump.find("INTEGER"), std::string::npos);
    EXPECT_NE(dump.find("42"), std::string::npos);
    EXPECT_NE(dump.find("0x2a"), std::string::npos);
}

TEST_F(TLVUtilsTestFixture, HEXDumpAdvancedTLVWithIndent)
{
    const std::string str = "tests";
    auto tlv = AdvancedTLV::CreateString(str);
    TLVUtils utils;
    std::string dump = utils.HEXDumpAdvancedTLV(*tlv, false, 4);

    std::cout << "Dump: <" << dump << ">\n";

    // Should have proper indentation
    EXPECT_NE(dump.find("    AdvancedTLV"), std::string::npos);
}

TEST_F(TLVUtilsTestFixture, VerboseHEXDump) 
{
    const std::string str = "AB";

    TLVUtils utils;
    auto tlv = AdvancedTLV::CreateString(str);
    std::string dump = utils.HEXDumpAdvancedTLV(*tlv, true);
    
    // Should contain hex dump in verbose mode
    EXPECT_NE(dump.find("Hex Dump:"), std::string::npos);
    EXPECT_NE(dump.find("41 42"), std::string::npos); // 'A' 'B' in hex
}

TEST_F(TLVUtilsTestFixture, NestedHEXDump) {
    auto nested = AdvancedTLV::CreateNested();
    nested->AddChild(AdvancedTLV::CreateInteger(100));
    nested->AddChild(AdvancedTLV::CreateString("child"));
    
    TLVUtils utils;
    std::string dump = utils.HEXDumpAdvancedTLV(*nested);
    
    EXPECT_NE(dump.find("NESTED"), std::string::npos);
    EXPECT_NE(dump.find("Children: 2"), std::string::npos);
    EXPECT_NE(dump.find("100"), std::string::npos);
    EXPECT_NE(dump.find("child"), std::string::npos);
}
