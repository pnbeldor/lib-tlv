#ifndef __TLVPARSER_H__
#define __TLVPARSER_H__

#include <stdexcept>

#include "AdvancedTLV.h"

class TLVParser {
public:
    TLVParser();
    virtual ~TLVParser();
    static std::unique_ptr<AdvancedTLV> Parse(const std::vector<uint8_t>& data, size_t& offset) {
        if (offset + 4 > data.size()) {
            throw std::runtime_error("Insufficient data for TLV header");
        }
        
        // Read type (2 bytes)
        Type type = static_cast<Type>((data[offset] << 8) | data[offset + 1]);
        offset += 2;
        
        // Read length (2 bytes)
        uint16_t length = (data[offset] << 8) | data[offset + 1];
        offset += 2;
        
        // Check if we have enough data for the value
        if (offset + length > data.size()) {
            throw std::runtime_error("Insufficient data for TLV value");
        }
        
        // Extract value
        std::vector<uint8_t> value(data.begin() + offset, data.begin() + offset + length);
        offset += length;
        
        return std::make_unique<AdvancedTLV>(type, value);
    }
    
    static std::vector<std::unique_ptr<AdvancedTLV>> ParseMultiple(const std::vector<uint8_t>& data) {
        std::vector<std::unique_ptr<AdvancedTLV>> result;
        size_t offset = 0;
        
        while (offset < data.size()) {
            result.push_back(Parse(data, offset));
        }
        
        return result;
    }
};

#endif  //__TLVPARSER_H__
