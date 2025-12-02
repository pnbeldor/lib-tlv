

#include "gtest/gtest.h"

#include "TLVParser.h"

class TLVParserTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        
    }

    void TearDown() override
    {

    }
};

// Test round-trip serialization/parsing
TEST_F(TLVParserTestFixture, SerializationParsingTests) {
    // Test integer round-trip
    auto originalInt = TLV::CreateInteger(-54321);
    auto serializedInt = originalInt->Serialize();
    
    size_t offset = 0;
    auto parsedInt = TLVParser::Parse(serializedInt, offset);
    EXPECT_EQ(parsedInt->GetType(), Type::INTEGER);
    EXPECT_EQ(parsedInt->AsInteger(), -54321);
    
    // Test string round-trip
    auto originalStr = TLV::CreateString("Round Trip Test");
    auto serializedStr = originalStr->Serialize();
    
    offset = 0;
    auto parsedStr = TLVParser::Parse(serializedStr, offset);
    EXPECT_EQ(parsedStr->GetType(), Type::STRING);
    EXPECT_EQ(parsedStr->AsString(), "Round Trip Test");
    
    // Test boolean round-trip
    auto originalBool = TLV::CreateBool(true);
    auto serializedBool = originalBool->Serialize();
    
    offset = 0;
    auto parsedBool = TLVParser::Parse(serializedBool, offset);
    EXPECT_EQ(parsedBool->GetType(), Type::BOOLEAN);
    EXPECT_TRUE(parsedBool->AsBoolean());
}

// Test multiple TLV parsing
TEST_F(TLVParserTestFixture, MultipleTLVParsingTests) {
    std::vector<uint8_t> message;
    
    // Create and serialize multiple TLVs
    auto tlv1 = TLV::CreateInteger(1);
    auto tlv2 = TLV::CreateString("test");
    auto tlv3 = TLV::CreateBool(false);
    
    auto data1 = tlv1->Serialize();
    auto data2 = tlv2->Serialize();
    auto data3 = tlv3->Serialize();
    
    message.insert(message.end(), data1.begin(), data1.end());
    message.insert(message.end(), data2.begin(), data2.end());
    message.insert(message.end(), data3.begin(), data3.end());
    
    // Parse all TLVs
    auto parsedTLVs = TLVParser::ParseMultiple(message);
    EXPECT_EQ(parsedTLVs.size(), 3);
    
    // Verify each TLV
    EXPECT_EQ(parsedTLVs[0]->GetType(), Type::INTEGER);
    EXPECT_EQ(parsedTLVs[0]->AsInteger(), 1);
    
    EXPECT_EQ(parsedTLVs[1]->GetType(), Type::STRING);
    EXPECT_EQ(parsedTLVs[1]->AsString(), "test");
    
    EXPECT_EQ(parsedTLVs[2]->GetType(), Type::BOOLEAN);
    EXPECT_FALSE(parsedTLVs[2]->AsBoolean());
}

// Test parsing with offset
TEST_F(TLVParserTestFixture, ParsingWithOffsetTests) {
    auto tlv1 = TLV::CreateInteger(100);
    auto tlv2 = TLV::CreateString("offset");
    
    auto data1 = tlv1->Serialize();
    auto data2 = tlv2->Serialize();
    
    std::vector<uint8_t> combined = data1;
    combined.insert(combined.end(), data2.begin(), data2.end());
    
    size_t offset = 0;
    auto parsed1 = TLVParser::Parse(combined, offset);
    auto parsed2 = TLVParser::Parse(combined, offset);
    
    EXPECT_EQ(parsed1->GetType(), Type::INTEGER);
    EXPECT_EQ(parsed2->GetType(), Type::STRING);
    EXPECT_EQ(offset, combined.size());  // Should consume all data
}
