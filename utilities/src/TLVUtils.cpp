/* --- TLVUtils.cpp --- */

/* ------------------------------------------
author: Pierre Nicolson Beldor
date: 11/20/2025
------------------------------------------ */
#include <iostream>
#include <sstream>
#include <iomanip>

#include "TLVUtils.h"

TLVUtils::TLVUtils() {
    // Constructor
}

TLVUtils::~TLVUtils() {
    // Destructor
}

void TLVUtils::printTLV(const std::vector<uint8_t>& data)
{
    size_t offset = 0;
    int index = 0;
    
    while (offset < data.size()) {
        try {
            auto tlv = TLVParser::Parse(data, offset);
            std::cout << "TLV " << index++ << ": Type=" << AdvancedTLV::TypeToString(tlv->GetType()) 
                        << ", Length=" << tlv->GetValue().size() << std::endl;
            
            // Print value based on type
            switch (tlv->GetType()) {
                case Type::INTEGER:
                    std::cout << "  Value (int): " << tlv->AsInteger() << std::endl;
                    break;
                case Type::STRING:
                    std::cout << "  Value (str): " << tlv->AsString() << std::endl;
                    break;
                case Type::BOOLEAN:
                    std::cout << "  Value (bool): " << (tlv->AsBoolean() ? "true" : "false") << std::endl;
                    break;
                default:
                    std::cout << "  Value (raw): ";
                    for (uint8_t byte : tlv->GetValue()) {
                        printf("%02X ", byte);
                    }
                    std::cout << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing TLV at offset " << offset << ": " << e.what() << std::endl;
            break;
        }
    }
}

std::vector<uint8_t> TLVUtils::CreateMessage(const std::vector<std::unique_ptr<AdvancedTLV>>& tlvs) {
    std::vector<uint8_t> result;
    for (const auto& tlv : tlvs) {
        auto data = tlv->Serialize();
        result.insert(result.end(), data.begin(), data.end());
    }
    return result;
}

std::string TLVUtils::TypeToString(Type type)
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

// Helper for valueToString with AdvancedTLV
std::string ValueToString(const AdvancedTLV& tlv) {
    try {
        switch (tlv.GetType()) {
            case Type::INTEGER:
                return std::to_string(tlv.AsInteger()) + " (0x" + 
                       [](int32_t val) { 
                           std::ostringstream oss; 
                           oss << std::hex << val; 
                           return oss.str(); 
                       }(tlv.AsInteger()) + ")";
            case Type::STRING:
                return "\"" + tlv.AsString() + "\"";
            case Type::BOOLEAN:
                return tlv.AsBoolean() ? "true" : "false";
            default:
                return "[raw data]";
        }
    } catch (const std::exception& e) {
        return std::string("[error: ") + e.what() + "]";
    }
}

std::string TLVUtils::hexDump(const std::vector<uint8_t>& data, int indent)
{
    if (data.empty()) return "";
    
    std::ostringstream oss;
    std::string indent_str(indent, ' ');
    const size_t bytes_per_line = 16;
    
    for (size_t i = 0; i < data.size(); i += bytes_per_line) {
        oss << indent_str;
        
        // Offset
        oss << std::hex << std::setw(4) << std::setfill('0') << i << ": ";
        
        // Hex bytes
        for (size_t j = 0; j < bytes_per_line; ++j) {
            if (i + j < data.size()) {
                oss << std::hex << std::setw(2) << std::setfill('0') 
                    << static_cast<int>(data[i + j]) << " ";
            } else {
                oss << "   ";
            }
        }
        
        oss << " ";
        
        // ASCII representation
        for (size_t j = 0; j < bytes_per_line && i + j < data.size(); ++j) {
            uint8_t byte = data[i + j];
            if (byte >= 32 && byte <= 126) {
                oss << static_cast<char>(byte);
            } else {
                oss << ".";
            }
        }
        
        oss << std::endl;
    }
    
    return oss.str();
}

std::string TLVUtils::HEXDumpData(const std::vector<uint8_t>& data, bool verbose)
{
    std::ostringstream oss;
    size_t offset = 0;
    int tlv_count = 0;
    
    oss << "TLV Data Dump (" << data.size() << " bytes total):" << std::endl;
    
    while (offset < data.size()) {
        try {
            oss << "TLV #" << tlv_count++ << " (offset: 0x" 
                << std::hex << offset << std::dec << "):" << std::endl;
            
            auto tlv = TLVParser::Parse(data, offset);
            oss << HEXDumpAdvancedTLV(*tlv, verbose, 2);
            oss << std::endl;
            
        } catch (const std::exception& e) {
            oss << "  ERROR parsing at offset 0x" << std::hex << offset 
                << std::dec << ": " << e.what() << std::endl;
            break;
        }
    }
    return oss.str();
}

// EXDump for basic TLV
std::string TLVUtils::HEXDumpTLV(const TLV& tlv, bool verbose, int indent) {
    std::ostringstream oss;
    std::string indent_str(indent, ' ');
    
    oss << indent_str << "TLV [" << TypeToString(tlv.GetType()) << "]" << std::endl;
    oss << indent_str << "  Type: " << static_cast<uint16_t>(tlv.GetType()) << " (0x" 
        << std::hex << std::setw(4) << std::setfill('0') << static_cast<uint16_t>(tlv.GetType()) << std::dec << ")" << std::endl;
    oss << indent_str << "  Length: " << tlv.GetValue().size() << " bytes" << std::endl;
    oss << indent_str << "  Value: " << tlv.AsString() << std::endl;
    
    if (verbose && !tlv.GetValue().empty()) {
        oss << indent_str << "  Hex Dump:" << std::endl;
        oss << hexDump(tlv.GetValue(), indent + 4);
    }
    
    if (!tlv.GetValue().empty()) {
        oss << indent_str << "  Raw: ";
        const auto& value = tlv.GetValue();
        for (size_t i = 0; i < std::min(value.size(), size_t(8)); ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') 
                << static_cast<int>(value[i]) << " ";
        }
        if (value.size() > 8) {
            oss << "... (" << (value.size() - 8) << " more bytes)";
        }
        oss << std::dec << std::endl;
    }
    
    return oss.str();
}

std::string TLVUtils::HEXDumpAdvancedTLV(const AdvancedTLV& tlv, bool verbose, int indent)
{
       std::ostringstream oss;
    std::string indent_str(indent, ' ');
    
    oss << indent_str << "AdvancedTLV [" << TypeToString(tlv.GetType()) << "]" << std::endl;
    oss << indent_str << "  Type: " << static_cast<uint16_t>(tlv.GetType()) << " (0x" 
        << std::hex << std::setw(4) << std::setfill('0') << static_cast<uint16_t>(tlv.GetType()) << std::dec << ")" << std::endl;
    
    if (tlv.GetType() == Type::ARRAY || tlv.GetType() == Type::NESTED_TLV) {
        oss << indent_str << "  Children: " << tlv.GetChildren().size() << " items" << std::endl;
        
        // Dump children
        for (size_t i = 0; i < tlv.GetChildren().size(); ++i) {
            oss << indent_str << "  [" << i << "]:" << std::endl;
            oss << HEXDumpAdvancedTLV(*tlv.GetChildren()[i], verbose, indent + 4);
        }
    } else {
        oss << indent_str << "  Length: " << tlv.GetValue().size() << " bytes" << std::endl;
        oss << indent_str << "  Value: " << ValueToString(tlv) << std::endl;
        
        if (verbose && !tlv.GetValue().empty()) {
            oss << indent_str << "  Hex Dump:" << std::endl;
            oss << hexDump(tlv.GetValue(), indent + 4);
        }
    }
    
    return oss.str();

}
