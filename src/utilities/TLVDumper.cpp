#include "TLVDumper.h"

#include "TLVDumper.h"
//#include "hex_utils.h"
#include "TLVPrinter.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <zlib.h>  // For checksum calculation, optional
//#include <base64.h> // You'll need a base64 library or implement it


bool TLVDumper::dump_to_file(const std::string& filename, const TLV& tlv) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    auto encoded = tlv.Serialize();
    file.write(reinterpret_cast<const char*>(encoded.data()), encoded.size());
    return file.good();
}

/*
bool TLVDumper::dump_to_file(const std::string& filename, const std::vector<TLV>& tlvs) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& tlv : tlvs) {
        auto encoded = tlv.Serialize();
        file.write(reinterpret_cast<const char*>(encoded.data()), encoded.size());
    }
    return file.good();
}

std::unique_ptr<TLV> TLVDumper::load_from_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return nullptr;
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        try {
            return std::make_unique<TLV>(tlv.Parse(buffer.data(), buffer.size()));
        } catch (const std::exception&) {
            return nullptr;
        }
    }
    return nullptr;
}

std::vector<TLV> TLVDumper::load_multiple_from_file(const std::string& filename) {
    std::vector<TLV> result;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return result;
    }
    
    // Read entire file
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    
    // Parse all TLVs from buffer
    size_t offset = 0;
    while (offset < buffer.size()) {
        try {
            TLV tlv = TLV::Parse(buffer.data() + offset, buffer.size() - offset);
            result.push_back(tlv);
            offset += tlv.get_total_length();
        } catch (const std::exception&) {
            // Couldn't parse further
            break;
        }
    }
    
    return result;
}

bool TLVDumper::dump_to_text_file(const std::string& filename, const TLV& tlv, bool include_ascii) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    PrintOptions options;
    options.color_output = false;
    options.show_ascii = include_ascii;
    
    TLVPrinter printer(options);
    file << printer.print(tlv);
    return file.good();
}

bool TLVDumper::dump_to_json_file(const std::string& filename, const TLV& tlv, bool pretty) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << TLVPrinter::to_json(tlv, pretty);
    return file.good();
}

bool TLVDumper::dump_to_xml_file(const std::string& filename, const TLV& tlv, bool pretty) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << TLVPrinter::to_xml(tlv, pretty);
    return file.good();
}

bool TLVDumper::compare_files(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);
    
    if (!f1.is_open() || !f2.is_open()) {
        return false;
    }
    
    // Compare file sizes first
    f1.seekg(0, std::ios::end);
    f2.seekg(0, std::ios::end);
    if (f1.tellg() != f2.tellg()) {
        return false;
    }
    
    // Compare content
    f1.seekg(0, std::ios::beg);
    f2.seekg(0, std::ios::beg);
    
    const size_t buffer_size = 4096;
    std::vector<char> buffer1(buffer_size);
    std::vector<char> buffer2(buffer_size);
    
    while (!f1.eof() && !f2.eof()) {
        f1.read(buffer1.data(), buffer_size);
        f2.read(buffer2.data(), buffer_size);
        
        if (std::memcmp(buffer1.data(), buffer2.data(), f1.gcount()) != 0) {
            return false;
        }
    }
    
    return true;
}

std::string TLVDumper::get_file_info(const std::string& filename) {
    std::ostringstream oss;
    
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return "File not found or cannot be opened";
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Read first few bytes to determine if it's valid TLV
    std::vector<uint8_t> header(16);
    file.read(reinterpret_cast<char*>(header.data()), header.size());
    
    oss << "Filename: " << filename << "\n";
    oss << "Size: " << size << " bytes\n";
    
    // Try to parse as TLV
    try {
        TLV tlv = TLV::Parse(header.data(), header.size());
        oss << "Valid TLV detected\n";
        oss << "Tag: 0x" << std::hex << tlv.GetType() << std::dec << "\n";
        oss << "Length: " << tlv.get_length() << " bytes\n";
        oss << "Total length: " << tlv.get_total_length() << " bytes\n";
        oss << "Constructed: " << (tlv.is_constructed() ? "Yes" : "No") << "\n";
    } catch (const std::exception&) {
        oss << "Not a valid TLV (or truncated)\n";
    }
    
    return oss.str();
}

uint32_t TLVDumper::calculate_checksum(const TLV& tlv) {
    auto encoded = tlv.Serialize();
    uint32_t checksum = 0;
    
    // Simple checksum calculation
    for (uint8_t byte : encoded) {
        checksum += byte;
    }
    
    return checksum;
}

bool TLVDumper::verify_checksum(const TLV& tlv, uint32_t expected_checksum) {
    return calculate_checksum(tlv) == expected_checksum;
}
*/

