# bel-tlv
The implementation and use of TLV (Type-Length-Value) encoding in C++. TLV is a simple binary encoding format commonly used in protocols and file formats.

## Key Features

  1. Type Safety: Strongly typed TLV structures
  2. Flexibility: Support for various data types
  3. Nested Structures: Ability to create complex nested TLV structures
  4. Error Handling: Proper exception handling for malformed data
  5. Memory Management: Smart pointers for automatic memory management
  6. Extensibility: Easy to add new TLV types

## Common Use Cases
  - Network protocols
  - Configuration files
  - Embedded systems communication
  - Database record formats

This implementation provides a solid foundation for working with TLV encoding in C++ and can be extended based on your specific requirements.

# Usage Examples
### 1. Build with GCC:
    - mkdir build && cd build
    - cmake .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

### 2. Build with Clang:
    -  mkdir build && cd build
    - cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
### 3. Build with MSVC:
    - cmake -G "Visual Studio 17 2022" -A x64 ..
### 4. Enable LTO and sanitizers:
    - cmake .. -DTLV_ENABLE_LTO=ON -DTLV_ENABLE_SANITIZERS=ON
