
#include <iostream>

#include "TLVUtils.h"
#include "TLVParser.h"

static void SimpleExamples()
{
    TLVUtils utils;
    std::cout << "\n=== Simple Examples ===" << std::endl;
    
    // Quick dumps without verbose mode
    TLV simple_int(Type::INTEGER, 255);
    std::cout << utils.HEXDumpTLV(simple_int) << std::endl;
    
    TLV simple_str(Type::STRING, "test");
    std::cout << utils.HEXDumpTLV(simple_str) << std::endl;
    
    auto simple_advanced = AdvancedTLV::CreateInteger(42);
    std::cout << utils.HEXDumpAdvancedTLV(*simple_advanced) << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::cout << "Hello from Simple examples\n";

    SimpleExamples();
    
    return 0;
}