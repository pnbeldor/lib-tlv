
#include "TLVUtils.h"

void parsingExample() {
    std::cout << "\n=== Parsing Example ===" << std::endl;
    
    // Create and serialize a TLV
    std::string msg = "Test Message";
    TLV original(Type::STRING, msg);
    auto data = original.Serialize();

    std::cout << "Original string value: \"Test Message\"" << std::endl;
    std::cout << "Serialized data (" << data.size() << " bytes): ";

    // Print hex representation
    std::cout << "Hex: ";
    for (uint8_t byte : data) {
        printf("%02X ", byte);
    }
    std::cout << std::endl;
    
    // Parse it back
    size_t offset = 0;
    try {
        auto parsed = TLVParser::Parse(data, offset);
        std::cout << "Parsed type: " << static_cast<int>(parsed->GetType()) << std::endl;
        std::cout << "Parsed value: " << parsed->AsString() << std::endl;
        std::cout << "Parsed value length: " << parsed->GetValue().size() << " bytes" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::cout << "Hello from advanced examples\n";

    parsingExample();
    return 0;
}
