
#include "TLVUtils.h"


static void arrayExample() {
    std::cout << "\n=== Array TLV Example ===" << std::endl;
    
    auto numbers = AdvancedTLV::CreateArray();
    
    for (int i = 1; i <= 8; ++i) {
        numbers->AddChild(AdvancedTLV::CreateInteger(i * 10));
    }
    
    auto serialized = numbers->Serialize();
    std::cout << "Array TLV size: " << serialized.size() << " bytes" << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::cout << "Hello from advanced examples\n";

    arrayExample();
    return 0;
}
