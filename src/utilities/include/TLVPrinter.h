#ifndef __TLVPRINTER_H__
#define __TLVPRINTER_H__


#include "TLV.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>

/**
 * @brief Options for TLV printing
 */
struct PrintOptions {
    int indent_size = 2;          // Number of spaces per indent level
    bool show_offset = true;      // Show byte offset
    bool show_hex = true;         // Show hex dump
    bool show_ascii = true;       // Show ASCII representation
    bool color_output = true;     // Use ANSI colors (if supported)
    size_t max_value_length = 64; // Maximum value length to display
    bool compact = false;         // Compact output format
};

/**
 * @brief ANSI color codes for terminal output
 */
namespace colors {
#ifdef _WIN32
    // Windows doesn't support ANSI colors by default in older versions
    static constexpr const char* reset = "";
    static constexpr const char* tag = "";
    static constexpr const char* length = "";
    static constexpr const char* value = "";
    static constexpr const char* offset = "";
    static constexpr const char* error = "";
    static constexpr const char* success = "";
#else
    static constexpr const char* reset = "\033[0m";
    static constexpr const char* tag = "\033[1;36m";     // Bright cyan
    static constexpr const char* length = "\033[1;33m";  // Bright yellow
    static constexpr const char* value = "\033[1;32m";   // Bright green
    static constexpr const char* offset = "\033[1;37m";  // Bright white
    static constexpr const char* error = "\033[1;31m";   // Bright red
    static constexpr const char* success = "\033[1;32m"; // Bright green
#endif
}

/**
 * @brief TLV Printer class for formatted TLV output
 */
class TLVPrinter {
public:
    TLVPrinter() = default;
    explicit TLVPrinter(const PrintOptions& options);
    
    // /**
    //  * @brief Print a single TLV object
    //  */
    // std::string print(const TLV& tlv, int indent_level = 0) const;
    
    // /**
    //  * @brief Print a vector of TLV objects
    //  */
    // std::string print(const std::vector<TLV>& tlvs, int indent_level = 0) const;
    
    // /**
    //  * @brief Print a TLV from raw bytes
    //  */
    // std::string print(const uint8_t* data, size_t length, int indent_level = 0) const;
    
    // /**
    //  * @brief Print to std::ostream
    //  */
    // void print(std::ostream& os, const TLV& tlv, int indent_level = 0) const;
    
    // /**
    //  * @brief Print to std::ostream
    //  */
    // void print(std::ostream& os, const std::vector<TLV>& tlvs, int indent_level = 0) const;
    
    // /**
    //  * @brief Set print options
    //  */
    // void set_options(const PrintOptions& options) { options_ = options; }
    
    // /**
    //  * @brief Get current print options
    //  */
    // const PrintOptions& get_options() const { return options_; }
    
    // /**
    //  * @brief Convert TLV to JSON string
    //  */
    // static std::string to_json(const TLV& tlv, bool pretty = true);
    
    // /**
    //  * @brief Convert TLV vector to JSON string
    //  */
    // static std::string to_json(const std::vector<TLV>& tlvs, bool pretty = true);
    
    // /**
    //  * @brief Convert TLV to XML string
    //  */
    // static std::string to_xml(const TLV& tlv, bool pretty = true);
    
    // /**
    //  * @brief Convert TLV vector to XML string
    //  */
    // static std::string to_xml(const std::vector<TLV>& tlvs, bool pretty = true);

private:
    PrintOptions options_;
/*    
    std::string indent(int level) const;
    std::string format_tag(uint32_t tag) const;
    std::string format_length(size_t length) const;
    std::string format_value(const std::vector<uint8_t>& value) const;
    std::string format_offset(size_t offset) const;
    std::string hex_dump(const uint8_t* data, size_t length) const;
    std::string ascii_dump(const uint8_t* data, size_t length) const;
    
    // JSON helpers
    static void to_json_internal(std::ostream& os, const TLV& tlv, int indent_level, bool pretty);
    static void to_json_internal(std::ostream& os, const std::vector<TLV>& tlvs, int indent_level, bool pretty);
    
    // XML helpers
    static void to_xml_internal(std::ostream& os, const TLV& tlv, int indent_level, bool pretty);
    static void to_xml_internal(std::ostream& os, const std::vector<TLV>& tlvs, int indent_level, bool pretty);
*/
};

// /**
//  * @brief Convenience function to print TLV to stdout
//  */
// void print_tlv(const TLV& tlv, const PrintOptions& options = PrintOptions());

// /**
//  * @brief Convenience function to print TLV vector to stdout
//  */
// void print_tlvs(const std::vector<TLV>& tlvs, const PrintOptions& options = PrintOptions());

// /**
//  * @brief Convenience function to get string representation
//  */
// std::string to_string(const TLV& tlv, const PrintOptions& options = PrintOptions());

// /**
//  * @brief Convenience function to get string representation of vector
//  */
// std::string to_string(const std::vector<TLV>& tlvs, const PrintOptions& options = PrintOptions());

#endif  //__TLVPRINTER_H__