/* --- AdvancedTLV.cpp --- */

/* ------------------------------------------
author: Pierre Nicolson Beldor
date: 11/20/2025
------------------------------------------ */
#include <iomanip>
#include <iostream>
#include <sstream>

#include "AdvancedTLV.h"

    AdvancedTLV::AdvancedTLV(Type type) : type_(type) {}
    AdvancedTLV::AdvancedTLV(Type type, const std::vector<uint8_t>& value) : type_(type), value_(value) {}
    // Value setters
void AdvancedTLV::SetInteger(int32_t value)
{
    value_.clear();
    for (int i = sizeof(int32_t) - 1; i >= 0; --i)
    {
        value_.push_back((value >> (i * 8)) & 0xFF);
    }
}

void AdvancedTLV::SetString(const std::string& value)
{
    value_.assign(value.begin(), value.end());
}

void AdvancedTLV::SetBoolean(bool value)
{
    value_.clear();
    value_.push_back(value ? 1 : 0);
}

void AdvancedTLV::AddChild(std::unique_ptr<AdvancedTLV> child)
{
    children_.push_back(std::move(child));
}

// Value converters
int32_t AdvancedTLV::AsInteger() const {
    if (value_.size() != sizeof(int32_t)) {
        throw std::runtime_error("Invalid integer size");
    }
    int32_t result = 0;
    for (uint8_t byte : value_) {
        result = (result << 8) | byte;
    }
    return result;
}

std::string AdvancedTLV::AsString() const
{
    return std::string(value_.begin(), value_.end());
}

bool AdvancedTLV::AsBoolean() const
{
    return !value_.empty() && value_[0] != 0;
}

// Serialization
std::vector<uint8_t> AdvancedTLV::Serialize() const
{
    std::vector<uint8_t> result;
    
    // Serialize type
    result.push_back((static_cast<uint16_t>(type_) >> 8) & 0xFF);
    result.push_back(static_cast<uint16_t>(type_) & 0xFF);
    
    // For nested types, serialize children first
    std::vector<uint8_t> value_data;
    if (type_ == Type::ARRAY || type_ == Type::NESTED_TLV) {
        for (const auto& child : children_) {
            auto child_data = child->Serialize();
            value_data.insert(value_data.end(), child_data.begin(), child_data.end());
        }
    } else {
        value_data = value_;
    }
    
    // Serialize length
    uint16_t length = static_cast<uint16_t>(value_data.size());
    result.push_back((length >> 8) & 0xFF);
    result.push_back(length & 0xFF);
    
    // Serialize value
    result.insert(result.end(), value_data.begin(), value_data.end());
    
    return result;
}

std::string AdvancedTLV::TypeToString(Type type)
{
    switch (type)
    {
        case Type::INTEGER: return "INTEGER";
        case Type::STRING: return "STRING";
        case Type::BOOLEAN: return "BOOLEAN";
        case Type::ARRAY: return "ARRAY";
        case Type::NESTED_TLV: return "NESTED_TLV";
        default: return "UNKNOWN";
    }
}

std::string AdvancedTLV::ValueToString() const
{
    try {
        switch (type_) {
            case Type::INTEGER:
                return std::to_string(AsInteger()) + " (0x" + ToHexString(AsInteger()) + ")";
            case Type::STRING:
                return "\"" + AsString() + "\"";
            case Type::BOOLEAN:
                return AsBoolean() ? "true" : "false";
            case Type::ARRAY:
            [[fallthrough]]; // Explicitly mark intentional fallthrough
            case Type::NESTED_TLV:
            [[fallthrough]]; // Explicitly mark intentional fallthrough
            default:
                return "[raw data]";
        }
    } catch (const std::exception& e) {
        return std::string("[error: ") + e.what() + "]";
    }
}

std::string AdvancedTLV::ToHexString(int32_t value) const
{
    std::ostringstream oss;
    oss << std::hex << value;
    return oss.str();
}

std::string AdvancedTLV::CompactHexDump(const std::vector<uint8_t>& data) const
{
    std::ostringstream oss;
    for (size_t i = 0; i < std::min(data.size(), size_t(8)); ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') 
            << static_cast<int>(data[i]) << " ";
    }
    if (data.size() > 8) {
        oss << "... (" << (data.size() - 8) << " more)";
    }
    return oss.str();
}
