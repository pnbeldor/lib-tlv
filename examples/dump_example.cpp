
#include "TLVUtils.h"
#include "TLV.h"

static void demonstrateEXDump() {
    std::cout << "=== TLV EXDump Demonstration ===" << std::endl;

    TLVUtils utils;
    // Basic TLV examples
    TLV int_tlv(Type::INTEGER, -12345);
    TLV string_tlv(Type::STRING, "Hello, World!");
    TLV bool_tlv(Type::BOOLEAN, true);

    std::cout << "1. Basic Integer TLV:" << std::endl;
    std::cout << utils.HEXDumpTLV(int_tlv) << std::endl;

    std::cout << "2. String TLV (verbose):" << std::endl;
    std::cout << utils.HEXDumpTLV(string_tlv, true) << std::endl;

    std::cout << "3. Boolean TLV:" << std::endl;
    std::cout << utils.HEXDumpTLV(bool_tlv) << std::endl;

    // Advanced TLV with nesting
    auto person = AdvancedTLV::CreateNested();
    person->AddChild(AdvancedTLV::CreateString("John Doe"));
    person->AddChild(AdvancedTLV::CreateInteger(30));
    person->AddChild(AdvancedTLV::CreateBoolean(false));

    std::cout << "4. Nested TLV Structure:" << std::endl;
    std::cout << utils.HEXDumpAdvancedTLV(*person) << std::endl;

    // Array example
    auto numbers = AdvancedTLV::CreateArray();
    for (int i = 1; i <= 5; ++i) {
        numbers->AddChild(AdvancedTLV::CreateInteger(i * 10));
    }

    std::cout << "5. Array TLV (verbose):" << std::endl;
    std::cout << utils.HEXDumpAdvancedTLV(*numbers, true) << std::endl;

    // Dump serialized data
    auto serialized_data = person->Serialize();
    std::cout << "6. Serialized Data Dump:" << std::endl;
    std::cout << utils.HEXDumpData(serialized_data, true) << std::endl;
}

/*
void testWithLargeData() {
    std::cout << "\n=== Large Data EXDump ===" << std::endl;

    // Create TLV with larger data
    std::string long_string = "This is a much longer string that will demonstrate ";
    long_string += "how the EXDump function handles larger amounts of data ";
    long_string += "by showing only a preview in non-verbose mode.";

    TLV large_tlv(TLV::STRING, long_string);

    std::cout << "Non-verbose (shows preview):" << std::endl;
    std::cout << large_tlv.EXDump(false) << std::endl;

    std::cout << "Verbose (shows full hex dump):" << std::endl;
    std::cout << large_tlv.EXDump(true) << std::endl;
}
*/

static void advancedExample()
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

    demonstrateEXDump();
    advancedExample();
    return 0;
}
