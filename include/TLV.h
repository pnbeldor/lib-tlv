#ifndef __TLV__
#define __TLV__

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

enum class Type : uint16_t
{
    INTEGER =1,
    STRING = 2,
    BOOLEAN = 3,
    ARRAY = 4,
    NESTED_TLV = 5
};

class TLV
{
public:
    static std::unique_ptr<TLV> CreateString(const std::string& value);
    static std::unique_ptr<TLV> CreateInteger(int32_t value);
    static std::unique_ptr<TLV> CreateBool(bool value);
    static std::unique_ptr<TLV> CreateRaw(Type type, const std::vector<uint8_t>& value);

    // Getters
    Type GetType() const { return type_; }
    const std::vector<uint8_t>& GetValue() const { return value_; }

    std::vector<uint8_t> Serialize() const;
    int32_t AsInteger() const;
    std::string AsString() const;
    bool AsBoolean() const;

    std::string TypeToString(Type type);

    TLV(Type type, const std::vector<uint8_t>& value);
    TLV(Type type, const std::string& value);
    explicit TLV(Type type, int32_t intValue);

private:
    Type type_;
    std::vector<uint8_t> value_;
};

#endif //__TLV__
