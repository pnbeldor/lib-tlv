

#include "gtest/gtest.h"

#include "AdvancedTLV.h"

class AdvancedTLVTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        
    }

    void TearDown() override
    {

    }
};

TEST_F(AdvancedTLVTestFixture, NestedTLVTest)
{
    auto nested = AdvancedTLV::CreateNested();
    nested->AddChild(AdvancedTLV::CreateInteger(42));
    nested->AddChild(AdvancedTLV::CreateString("nested"));
    nested->AddChild(AdvancedTLV::CreateBoolean(true));
    
    auto serialized = nested->Serialize();
    
    // Should be able to serialize nested structure
    EXPECT_FALSE(serialized.empty());
    EXPECT_GT(serialized.size(), 10);  // Should be larger than a single TLV
}

// Test array TLV
TEST_F(AdvancedTLVTestFixture, ArrayTLVTests) {
    auto array = AdvancedTLV::CreateArray();
    array->AddChild(AdvancedTLV::CreateInteger(1));
    array->AddChild(AdvancedTLV::CreateInteger(2));
    array->AddChild(AdvancedTLV::CreateInteger(3));
    
    auto serialized = array->Serialize();
    EXPECT_FALSE(serialized.empty());
    
    // Verify we have 3 children
    EXPECT_EQ(array->GetChildren().size(), 3);
}
