/* --- AdvancedTLV.h --- */

/* ------------------------------------------
Author: Pierre Nicolson Beldor
Date: 11/20/2025
------------------------------------------ */
#ifndef __ADVANCED_TLV_H__
#define __ADVANCED_TLV_H__

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "TLV.h"

class AdvancedTLV {

public:
    AdvancedTLV(Type type);
    AdvancedTLV(Type type, const std::vector<uint8_t>& value);
    
    // Factory methods
    static std::unique_ptr<AdvancedTLV> CreateInteger(int32_t value) {
        auto tlv = std::make_unique<AdvancedTLV>(Type::INTEGER);
        tlv->SetInteger(value);
        return tlv;
    }
    
    static std::unique_ptr<AdvancedTLV> CreateString(const std::string& value) {
        auto tlv = std::make_unique<AdvancedTLV>(Type::STRING);
        tlv->SetString(value);
        return tlv;
    }
    
    static std::unique_ptr<AdvancedTLV> CreateBoolean(bool value) {
        auto tlv = std::make_unique<AdvancedTLV>(Type::BOOLEAN);
        tlv->SetBoolean(value);
        return tlv;
    }
    
    static std::unique_ptr<AdvancedTLV> CreateArray() {
        return std::make_unique<AdvancedTLV>(Type::ARRAY);
    }
    
    static std::unique_ptr<AdvancedTLV> CreateNested() {
        return std::make_unique<AdvancedTLV>(Type::NESTED_TLV);
    }
    
    // Value setters
    void SetInteger(int32_t value);
    
    void SetString(const std::string& value);
    
    void SetBoolean(bool value);
    
    // Children management
    void AddChild(std::unique_ptr<AdvancedTLV> child);
    
    // Serialization
    std::vector<uint8_t> Serialize() const;
    
    // Getters
    Type GetType() const { return type_; }
    const std::vector<uint8_t>& GetValue() const { return value_; }
    const std::vector<std::unique_ptr<AdvancedTLV>>& GetChildren() const { return children_; }
    
    // Value converters
    int32_t AsInteger() const;
    
    std::string AsString() const;
    bool AsBoolean() const;

    std::string HEXDump(bool verbose = false, int indent = 0) const;
    static std::string TypeToString(Type type);

    size_t CalculateTotalSize() const {
        if (type_ == Type::ARRAY || type_ == Type::NESTED_TLV) {
            size_t total = 0;
            for (const auto& child : children_) {
                total += child->CalculateTotalSize();
        }
            return total;
        }
        return value_.size();
    }
    
private:
    std::string ValueToString() const;
    std::string ToHexString(int32_t value) const;
    std::string CompactHexDump(const std::vector<uint8_t>& data) const;

private:
    Type type_;
    std::vector<uint8_t> value_;
    std::vector<std::unique_ptr<AdvancedTLV>> children_;
};

#endif // __ADVANCED_TLV_H__
