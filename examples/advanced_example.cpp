
#include "TLVUtils.h"

void advancedExample()
{
    std::cout << "\n=== Advanced TLV Example ===" << std::endl;
    
    // Create a nested structure
    auto person = AdvancedTLV::CreateNested();
    
    auto name = AdvancedTLV::CreateString("Pierre Nicolson Beldor, Pierre-Nicolson Michael II Beldor");
    auto age = AdvancedTLV::CreateInteger(30);
    auto isStudent = AdvancedTLV::CreateBoolean(false);
    
    person->AddChild(std::move(name));
    person->AddChild(std::move(age));
    person->AddChild(std::move(isStudent));
    
    // Serialize the nested structure
    auto serialized = person->Serialize();
    std::cout << "Nested TLV size: " << serialized.size() << " bytes" << std::endl;
    
    // Print hex representation
    std::cout << "Hex: ";
    for (uint8_t byte : serialized) {
        printf("%02X ", byte);
    }
    std::cout << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::cout << "Hello from advanced examples\n";

    advancedExample();
    return 0;
}
