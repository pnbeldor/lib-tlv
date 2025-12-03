#include "TLVPrinter.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <regex>

TLVPrinter::TLVPrinter(const PrintOptions& options) : options_(options) {}
/*
std::string TLVPrinter::print(const TLV& tlv, int indent_level) const {
    std::ostringstream oss;
    print(oss, tlv, indent_level);
    return oss.str();
}

std::string TLVPrinter::print(const std::vector<TLV>& tlvs, int indent_level) const {
    std::ostringstream oss;
    print(oss, tlvs, indent_level);
    return oss.str();
}

std::string TLVPrinter::print(const uint8_t* data, size_t length, int indent_level) const {
    try {
        TLV tlv = TLV::Parse(data, length);
        return print(tlv, indent_level);
    } catch (const std::exception& e) {
        std::ostringstream oss;
        if (options_.color_output) {
            oss << colors::error << "Invalid TLV data: " << e.what() << colors::reset;
        } else {
            oss << "Invalid TLV data: " << e.what();
        }
        return oss.str();
    }
}

void TLVPrinter::print(std::ostream& os, const TLV& tlv, int indent_level) const {
    std::string indent_str = indent(indent_level);
    
    // Print offset if enabled
    if (options_.show_offset && indent_level == 0) {
        os << indent_str << format_offset(0) << " ";
    }
    
    // Print tag
    os << format_tag(static_cast<uint16_t>(tlv.GetType()));
    
    // Print length
    os << " " << format_length(tlv.GetValue().size()) << " ";
    
    // Print value
    const auto& value = tlv.GetValue();
    if (!value.empty()) {
        os << format_value(value);
        
        // Print hex dump if enabled and value is not too long
        if (options_.show_hex && value.size() <= options_.max_value_length) {
            os << " " << hex_dump(value.data(), value.size());
            
            // Print ASCII dump if enabled
            if (options_.show_ascii) {
                os << " " << ascii_dump(value.data(), value.size());
            }
        }
    } else {
        os << "(empty)";
    }
    
    os << "\n";
    
    // Recursively print constructed TLVs
    if (tlv.is_constructed()) {
        try {
            auto nested = TLV::parse_constructed(tlv.get_value());
            for (const auto& nested_tlv : nested) {
                print(os, nested_tlv, indent_level + 1);
            }
        } catch (const std::exception&) {
            // Not a constructed TLV or parse error
        }
    }
}

void TLVPrinter::print(std::ostream& os, const std::vector<TLV>& tlvs, int indent_level) const {
    size_t offset = 0;
    for (const auto& tlv : tlvs) {
        if (options_.show_offset) {
            os << indent(indent_level) << format_offset(offset) << " ";
        }
        print(os, tlv, indent_level);
        offset += tlv.get_total_length();
    }
}

std::string TLVPrinter::indent(int level) const {
    if (level <= 0) return "";
    return std::string(level * options_.indent_size, ' ');
}

std::string TLVPrinter::format_tag(uint32_t tag) const {
    std::ostringstream oss;
    if (options_.color_output) {
        oss << colors::tag;
    }
    oss << "TAG: 0x" << std::hex << std::uppercase << std::setfill('0');
    
    // Format tag based on size
    if (tag <= 0xFF) {
        oss << std::setw(2) << tag;
    } else if (tag <= 0xFFFF) {
        oss << std::setw(4) << tag;
    } else if (tag <= 0xFFFFFF) {
        oss << std::setw(6) << tag;
    } else {
        oss << std::setw(8) << tag;
    }
    
    oss << std::dec;
    if (options_.color_output) {
        oss << colors::reset;
    }
    return oss.str();
}

std::string TLVPrinter::format_length(size_t length) const {
    std::ostringstream oss;
    if (options_.color_output) {
        oss << colors::length;
    }
    oss << "LEN: " << length;
    if (options_.color_output) {
        oss << colors::reset;
    }
    return oss.str();
}

std::string TLVPrinter::format_value(const std::vector<uint8_t>& value) const {
    std::ostringstream oss;
    if (options_.color_output) {
        oss << colors::value;
    }
    
    if (value.size() <= options_.max_value_length) {
        // Try to interpret as string if printable
        bool is_printable = std::all_of(value.begin(), value.end(),
            [](uint8_t c) { return std::isprint(c) || std::isspace(c); });
        
        if (is_printable && !value.empty()) {
            oss << "VAL: \"" << std::string(value.begin(), value.end()) << "\"";
        } else {
            oss << "VAL: [" << value.size() << " bytes]";
        }
    } else {
        oss << "VAL: [" << value.size() << " bytes, truncated]";
    }
    
    if (options_.color_output) {
        oss << colors::reset;
    }
    return oss.str();
}

std::string TLVPrinter::format_offset(size_t offset) const {
    std::ostringstream oss;
    if (options_.color_output) {
        oss << colors::offset;
    }
    oss << "@" << std::setw(4) << std::setfill('0') << offset;
    if (options_.color_output) {
        oss << colors::reset;
    }
    return oss.str();
}

std::string TLVPrinter::hex_dump(const uint8_t* data, size_t length) const {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < length; ++i) {
        if (i > 0) oss << " ";
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(data[i]);
    }
    oss << "]";
    return oss.str();
}

std::string TLVPrinter::ascii_dump(const uint8_t* data, size_t length) const {
    std::ostringstream oss;
    oss << "\"";
    for (size_t i = 0; i < length; ++i) {
        char c = static_cast<char>(data[i]);
        if (std::isprint(c) && !std::isspace(c)) {
            oss << c;
        } else {
            oss << ".";
        }
    }
    oss << "\"";
    return oss.str();
}

// JSON conversion methods
std::string TLVPrinter::to_json(const TLV& tlv, bool pretty) {
    std::ostringstream oss;
    to_json_internal(oss, tlv, 0, pretty);
    return oss.str();
}

std::string TLVPrinter::to_json(const std::vector<TLV>& tlvs, bool pretty) {
    std::ostringstream oss;
    to_json_internal(oss, tlvs, 0, pretty);
    return oss.str();
}

void TLVPrinter::to_json_internal(std::ostream& os, const TLV& tlv, int indent_level, bool pretty) {
    std::string indent_str = pretty ? std::string(indent_level * 2, ' ') : "";
    std::string nl = pretty ? "\n" : "";
    
    os << indent_str << "{" << nl;
    
    if (pretty) indent_level++;
    indent_str = std::string(indent_level * 2, ' ');
    
    os << indent_str << "\"tag\": \"" << std::hex << "0x" 
       << std::setw(8) << std::setfill('0') << tlv.GetType() << "\"," << nl;
    os << indent_str << "\"tag_dec\": " << std::dec << tlv.get_tag() << "," << nl;
    os << indent_str << "\"length\": " << tlv.GetValue().size() << "," << nl;
    os << indent_str << "\"constructed\": " << (tlv.is_constructed() ? "true" : "false") << "," << nl;
    
    const auto& value = tlv.GetValue();
    os << indent_str << "\"value_hex\": \"";
    for (size_t i = 0; i < value.size(); ++i) {
        if (i > 0) os << " ";
        os << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(value[i]);
    }
    os << "\"," << nl;
    
    // Try to decode as UTF-8 string
    bool is_printable = std::all_of(value.begin(), value.end(),
        [](uint8_t c) { return std::isprint(c) || std::isspace(c); });
    
    if (is_printable && !value.empty()) {
        std::string str_value(value.begin(), value.end());
        // Escape special JSON characters
        str_value = std::regex_replace(str_value, std::regex("\""), "\\\"");
        str_value = std::regex_replace(str_value, std::regex("\\\\"), "\\\\");
        str_value = std::regex_replace(str_value, std::regex("\n"), "\\n");
        str_value = std::regex_replace(str_value, std::regex("\r"), "\\r");
        str_value = std::regex_replace(str_value, std::regex("\t"), "\\t");
        
        os << indent_str << "\"value_string\": \"" << str_value << "\"," << nl;
    }
    
    os << indent_str << "\"total_length\": " << tlv.get_total_length();
    
    // Recursively add nested TLVs if constructed
    if (tlv.is_constructed()) {
        os << "," << nl << indent_str << "\"nested\": [" << nl;
        try {
            auto nested = TLV::parse_constructed(tlv.get_value());
            for (size_t i = 0; i < nested.size(); ++i) {
                if (i > 0) os << "," << nl;
                to_json_internal(os, nested[i], indent_level + 1, pretty);
            }
            if (!nested.empty()) os << nl;
            os << indent_str << "]";
        } catch (const std::exception&) {
            os << "]";
        }
    }
    
    if (pretty) {
        indent_level--;
        indent_str = std::string(indent_level * 2, ' ');
        os << nl << indent_str << "}";
    } else {
        os << "}";
    }
}

void TLVPrinter::to_json_internal(std::ostream& os, const std::vector<TLV>& tlvs, int indent_level, bool pretty) {
    std::string indent_str = pretty ? std::string(indent_level * 2, ' ') : "";
    std::string nl = pretty ? "\n" : "";
    
    os << indent_str << "[" << nl;
    
    for (size_t i = 0; i < tlvs.size(); ++i) {
        if (i > 0) os << "," << nl;
        to_json_internal(os, tlvs[i], indent_level + 1, pretty);
    }
    
    if (!tlvs.empty() && pretty) os << nl;
    os << indent_str << "]";
}

// XML conversion methods
std::string TLVPrinter::to_xml(const TLV& tlv, bool pretty) {
    std::ostringstream oss;
    to_xml_internal(oss, tlv, 0, pretty);
    return oss.str();
}

std::string TLVPrinter::to_xml(const std::vector<TLV>& tlvs, bool pretty) {
    std::ostringstream oss;
    oss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << (pretty ? "\n" : "");
    oss << "<tlvs>" << (pretty ? "\n" : "");
    for (const auto& tlv : tlvs) {
        to_xml_internal(oss, tlv, 1, pretty);
    }
    oss << "</tlvs>";
    return oss.str();
}

void TLVPrinter::to_xml_internal(std::ostream& os, const TLV& tlv, int indent_level, bool pretty) {
    std::string indent_str = pretty ? std::string(indent_level * 2, ' ') : "";
    std::string nl = pretty ? "\n" : "";
    
    os << indent_str << "<tlv tag=\"0x" << std::hex << std::setw(8) 
       << std::setfill('0') << tlv.get_tag() << "\"" << std::dec
       << " length=\"" << tlv.get_length() << "\""
       << " constructed=\"" << (tlv.is_constructed() ? "true" : "false") << "\""
       << " total_length=\"" << tlv.get_total_length() << "\">" << nl;
    
    const auto& value = tlv.GetValue();
    if (!value.empty()) {
        bool is_printable = std::all_of(value.begin(), value.end(),
            [](uint8_t c) { return std::isprint(c) || std::isspace(c); });
        
        if (is_printable) {
            std::string str_value(value.begin(), value.end());
            // Escape XML special characters
            str_value = std::regex_replace(str_value, std::regex("&"), "&amp;");
            str_value = std::regex_replace(str_value, std::regex("<"), "&lt;");
            str_value = std::regex_replace(str_value, std::regex(">"), "&gt;");
            str_value = std::regex_replace(str_value, std::regex("\""), "&quot;");
            str_value = std::regex_replace(str_value, std::regex("'"), "&apos;");
            
            os << indent_str << "  <value type=\"string\">" << str_value << "</value>" << nl;
        }
        
        os << indent_str << "  <value type=\"hex\">";
        for (size_t i = 0; i < value.size(); ++i) {
            if (i > 0) os << " ";
            os << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(value[i]);
        }
        os << std::dec << "</value>" << nl;
    }
    
    // Recursively add nested TLVs if constructed
    if (tlv.is_constructed()) {
        os << indent_str << "  <nested>" << nl;
        try {
            auto nested = TLV::parse_constructed(tlv.get_value());
            for (const auto& nested_tlv : nested) {
                to_xml_internal(os, nested_tlv, indent_level + 2, pretty);
            }
        } catch (const std::exception&) {
            // Not a constructed TLV
        }
        os << indent_str << "  </nested>" << nl;
    }
    
    os << indent_str << "</tlv>" << nl;
}

// Convenience functions
void print_tlv(const TLV& tlv, const PrintOptions& options) {
    TLVPrinter printer(options);
    std::cout << printer.print(tlv) << std::flush;
}

void print_tlvs(const std::vector<TLV>& tlvs, const PrintOptions& options) {
    TLVPrinter printer(options);
    std::cout << printer.print(tlvs) << std::flush;
}

std::string to_string(const TLV& tlv, const PrintOptions& options) {
    TLVPrinter printer(options);
    return printer.print(tlv);
}

std::string to_string(const std::vector<TLV>& tlvs, const PrintOptions& options) {
    TLVPrinter printer(options);
    return printer.print(tlvs);
}
*/
