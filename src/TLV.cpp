#include <iostream>

#include "TLV.h"

TLV::TLV(Type type, const std::vector<uint8_t>& value)
    : type_(type), value_(value)
{
}

TLV::TLV(Type type, const std::string& strValue)
    : type_(type)
{
    value_.assign(strValue.begin(), strValue.end());
}

TLV::TLV(Type type, int32_t value)
    : type_(type)
{
    if (type_ == Type::BOOLEAN)
    {
        value_.push_back(value ? 1 : 0);
    }
    else
    {
        // Convert integer to bytes (big-endian)
        for (int i = sizeof(int32_t) - 1; i >= 0; --i) {
            value_.push_back((value >> (i * 8)) & 0xFF);
        }
    }
}

std::unique_ptr<TLV> TLV::CreateString(const std::string& value)
{
    return std::make_unique<TLV>(Type::STRING, value);
}

std::unique_ptr<TLV> TLV::CreateInteger(int32_t value)
{
    return std::make_unique<TLV>(Type::INTEGER, value);
}

std::unique_ptr<TLV> TLV::CreateBool(bool value)
{
    return std::make_unique<TLV>(Type::BOOLEAN, value);
}

std::unique_ptr<TLV> TLV::CreateRaw(Type type, const std::vector<uint8_t>& value)
{
    return std::make_unique<TLV>(type, value);
}

// Serialize to bytes
std::vector<uint8_t> TLV::Serialize() const
{
    std::vector<uint8_t> result;
    
    // Type (2 bytes)
    result.push_back((static_cast<uint16_t>(type_) >> 8) & 0xFF);
    result.push_back(static_cast<uint16_t>(type_) & 0xFF);
    
    // Length (2 bytes)
    uint16_t length = static_cast<uint16_t>(value_.size());
    result.push_back((length >> 8) & 0xFF);
    result.push_back(length & 0xFF);
    
    // Value
    result.insert(result.end(), value_.begin(), value_.end());
    
    return result;
}

//Convert to specific types
int32_t TLV::AsInteger() const
{
    if (value_.size() != sizeof(int32_t)) {
        throw std::runtime_error("Invalid integer size");
    }
    
    int32_t result = 0;
    for (size_t i = 0; i < value_.size(); ++i) {
        result = (result << 8) | value_[i];
    }
    return result;
}

std::string TLV::AsString() const
{
    return std::string(value_.begin(), value_.end());
}

bool TLV::AsBoolean() const
{
    if (value_.empty()) return false;
    return value_[0] != 0;
}
