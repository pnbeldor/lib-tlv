

#include "gtest/gtest.h"

#include "TLV.h"
#include "AdvancedTLV.h"
#include "TLVParser.h"

class TLVMemoryPerformanceTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        
    }

    void TearDown() override
    {

    }
};

TEST_F(TLVMemoryPerformanceTestFixture, TLVLargeDataTest)
{
    std::string largeString(1000, 'X');  // 1000 character string
    auto largeTLV = TLV::CreateString(largeString);
    
    auto serialized = largeTLV->Serialize();
    EXPECT_EQ(serialized.size(), 4 + 1000);  // header + data
    
    size_t offset = 0;
    auto parsed = TLVParser::Parse(serialized, offset);
    EXPECT_EQ(parsed->AsString(), largeString);
}

// Test memory management
TEST_F(TLVMemoryPerformanceTestFixture, TLVMemoryManagementTest) {
    // Test that unique_ptr works correctly
    auto tlvPtr = TLV::CreateInteger(123);
    EXPECT_EQ(tlvPtr->GetType(), Type::INTEGER);
    
    // Test moving
    auto tlvPtr2 = std::move(tlvPtr);
    EXPECT_EQ(tlvPtr2->GetType(), Type::INTEGER);
    EXPECT_EQ(tlvPtr.get(), nullptr);  // Original should be null after move
}