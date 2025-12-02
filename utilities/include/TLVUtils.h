/* --- TLVUtils.h --- */

/* ------------------------------------------
Author: Pierre Nicolson Beldor
Date: 11/20/2025
------------------------------------------ */

#ifndef __TLVUTILS_H__
#define __TLVUTILS_H__

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "AdvancedTLV.h"
#include "TLVParser.h"

// Helper functions for working with TLV
/*
The EXDump function provides comprehensive debugging information with features like:
    1. Type names instead of just numbers
    2. Multiple output formats (compact vs verbose)
    3. Hex dumps with ASCII representation
    4. Nested structure visualization
    5. Error handling for malformed data
    6. Configurable indentation for hierarchy
*/
class TLVUtils {
public:

    TLVUtils();
    virtual ~TLVUtils();
    static void printTLV(const std::vector<uint8_t>& data);
    std::string ValueToString(const AdvancedTLV& tlv);

    static std::vector<uint8_t> CreateMessage(const std::vector<std::unique_ptr<AdvancedTLV>>& tlvs);
    std::string TypeToString(Type type);
    std::string hexDump(const std::vector<uint8_t>& data, int indent = 0);
    std::string HEXDumpData(const std::vector<uint8_t>& tlv_data, bool verbose = false);
    std::string HEXDumpAdvancedCollection(const std::vector<std::unique_ptr<AdvancedTLV>>& tlvs, bool verbose = false);
    std::string HEXDumpAdvancedTLV(const AdvancedTLV& tlv, bool verbose = false, int indent = 0);
    std::string HEXDumpTLV(const TLV& tlv, bool verbose = false, int indent = 0);
};
#endif // __TLVUTILS_H__
