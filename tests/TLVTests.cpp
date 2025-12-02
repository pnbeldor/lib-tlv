

#include "gtest/gtest.h"

#include "TLV.h"

class TLVTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        
    }

    void TearDown() override
    {

    }
};

TEST_F(TLVTestFixture, FactoryMethodsTest)
{
    auto intTLV = TLV::CreateInteger(42);

    EXPECT_EQ(intTLV->GetType(), Type::INTEGER);
    EXPECT_EQ(intTLV->AsInteger(), 42);

    auto stringTLV = TLV::CreateString("Hello");

    EXPECT_EQ(stringTLV->GetType(), Type::STRING);
    EXPECT_EQ(stringTLV->AsString(), "Hello");

    auto boolTLV = TLV::CreateBool(true);

    EXPECT_EQ(boolTLV->GetType(), Type::BOOLEAN);
    EXPECT_TRUE(boolTLV->AsBoolean());

    auto boolTLV2 = TLV::CreateBool(false);

    EXPECT_EQ(boolTLV2->GetType(), Type::BOOLEAN);
    EXPECT_FALSE(boolTLV2->AsBoolean());
}

TEST_F(TLVTestFixture, EdgeCaseValuesTest)
{
    // Test zero integer
    auto intTLV = TLV::CreateInteger(0);

    EXPECT_EQ(intTLV->GetType(), Type::INTEGER);
    EXPECT_EQ(intTLV->AsInteger(), 0);

    // Test negative integer
    auto intTLVNeg = TLV::CreateInteger(-123);

    EXPECT_EQ(intTLVNeg->GetType(), Type::INTEGER);
    EXPECT_EQ(intTLVNeg->AsInteger(), -123);

    // Test empty string
    auto stringTLV = TLV::CreateString("");

    EXPECT_EQ(stringTLV->GetType(), Type::STRING);
    EXPECT_EQ(stringTLV->AsString(), "");
    EXPECT_TRUE(stringTLV->GetValue().empty());

    // Test false boolean
    auto boolTLV = TLV::CreateBool(false);

    EXPECT_EQ(boolTLV->GetType(), Type::BOOLEAN);
    EXPECT_FALSE(boolTLV->AsBoolean());
}

// Test serialization format
TEST_F(TLVTestFixture, SerializationFormatTests)
{
    auto stringTlv = TLV::CreateString("AB");
    auto data = stringTlv->Serialize();
    
    // Should have: Type(2) + Length(2) + Value(2) = 6 bytes
    EXPECT_EQ(data.size(), 6);
    
    // Verify type field (STRING = 2)
    EXPECT_EQ(data[0], 0x00);
    EXPECT_EQ(data[1], 0x02);
    
    // Verify length field (2 bytes)
    EXPECT_EQ(data[2], 0x00);
    EXPECT_EQ(data[3], 0x02);
    
    // Verify value field (ASCII for "AB")
    EXPECT_EQ(data[4], 'A');
    EXPECT_EQ(data[5], 'B');
}

TEST_F(TLVTestFixture, IntegerSerializationTests)
{
    auto intTLV = TLV::CreateInteger(0x12345678);
    auto data = intTLV->Serialize();

    // Should have: Type(2) + Length(2) + Value(4) = 8 bytes
    EXPECT_EQ(data.size(), 8);
    
    // Verify type field (INTEGER = 1)
    EXPECT_EQ(data[0], 0x00);
    EXPECT_EQ(data[1], 0x01);
    
    // Verify length field (4 bytes)
    EXPECT_EQ(data[2], 0x00);
    EXPECT_EQ(data[3], 0x04);

    // Verify integer is stored in big-endian
    EXPECT_EQ(data[4], 0x12);
    EXPECT_EQ(data[5], 0x34);
    EXPECT_EQ(data[6], 0x56);
    EXPECT_EQ(data[7], 0x78);
}

// Test boolean serialization
TEST_F(TLVTestFixture, BooleanSerializationTests)
{
    auto trueTLV = TLV::CreateBool(true);
    auto trueData = trueTLV->Serialize();

    // Should have: Type(2) + Length(2) + Value(1) = 5 bytes
    EXPECT_EQ(trueData.size(), 5);
    
    // Verify type field (BOOLEAN = 3)
    EXPECT_EQ(trueData[0], 0x00);
    EXPECT_EQ(trueData[1], 0x03);
    
    // Verify length field (1 bytes)
    EXPECT_EQ(trueData[2], 0x00);
    EXPECT_EQ(trueData[3], 0x01);

    EXPECT_EQ(trueData[4], 0x01);  // Boolean true = 0x01
    
    auto falseTLV = TLV::CreateBool(false);
    auto falseData = falseTLV->Serialize();

    EXPECT_EQ(falseData[4], 0x00);  // Boolean false = 0x00
}
