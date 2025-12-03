#ifndef __TLVDUMPER_H__
#define __TLVDUMPER_H__

#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "TLV.h"

/**
 * @brief TLV Dumper for file and stream operations
 */
class TLVDumper {
public:
    /**
     * @brief Dump TLV to file in binary format
     */
    static bool dump_to_file(const std::string& filename, const TLV& tlv);
    
    /**
     * @brief Dump multiple TLVs to file in binary format
     */
    static bool dump_to_file(const std::string& filename, const std::vector<TLV>& tlvs);
    
    /**
     * @brief Load TLV from binary file
     */
    static std::unique_ptr<TLV> load_from_file(const std::string& filename);
    
    // /**
    //  * @brief Load multiple TLVs from binary file
    //  */
    // static std::vector<TLV> load_multiple_from_file(const std::string& filename);
    
    // /**
    //  * @brief Dump TLV to file in text format (hex dump)
    //  */
    // static bool dump_to_text_file(const std::string& filename, const TLV& tlv, 
    //                               bool include_ascii = true);
    
    // /**
    //  * @brief Dump TLV to file in JSON format
    //  */
    // static bool dump_to_json_file(const std::string& filename, const TLV& tlv,
    //                               bool pretty = true);
    
    // /**
    //  * @brief Dump TLV to file in XML format
    //  */
    // static bool dump_to_xml_file(const std::string& filename, const TLV& tlv,
    //                              bool pretty = true);
    
    // /**
    //  * @brief Compare two TLV files (binary comparison)
    //  */
    // static bool compare_files(const std::string& file1, const std::string& file2);
    
    // /**
    //  * @brief Get file information (size, modification time, etc.)
    //  */
    // static std::string get_file_info(const std::string& filename);
    
    // /**
    //  * @brief Calculate checksum of TLV data
    //  */
    // static uint32_t calculate_checksum(const TLV& tlv);
    
    // /**
    //  * @brief Verify checksum of TLV data
    //  */
    // static bool verify_checksum(const TLV& tlv, uint32_t expected_checksum);
    
    // /**
    //  * @brief Split large TLV into smaller chunks
    //  */
    // static std::vector<TLV> split_into_chunks(const TLV& tlv, size_t max_chunk_size);
    
    // /**
    //  * @brief Merge chunks back into original TLV
    //  */
    // static std::unique_ptr<TLV> merge_chunks(const std::vector<TLV>& chunks);
    
    // /**
    //  * @brief Convert TLV to base64 string
    //  */
    // static std::string to_base64(const TLV& tlv);
    
    // /**
    //  * @brief Convert base64 string to TLV
    //  */
    // static std::unique_ptr<TLV> from_base64(const std::string& base64_str);
    
    // /**
    //  * @brief Create TLV from hex string
    //  */
    // static std::unique_ptr<TLV> from_hex_string(const std::string& hex_str);
    
    // /**
    //  * @brief Convert TLV to hex string
    //  */
    // static std::string to_hex_string(const TLV& tlv, bool spaced = true);
    
    // /**
    //  * @brief Create TLV from ASCII string
    //  */
    // static std::unique_ptr<TLV> from_ascii_string(const std::string& ascii_str, uint32_t tag = 0x01);
    
    // /**
    //  * @brief Convert TLV to ASCII string (if possible)
    //  */
    // static std::string to_ascii_string(const TLV& tlv);
};

#endif  //__TLVDUMPER_H__