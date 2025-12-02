#!/bin/bash

# Cross-platform build script

BUILD_TYPE=${1:-Release}
BUILD_DIR="build-${BUILD_TYPE}"

echo "Building TLV Library (${BUILD_TYPE})..."

# Clean previous build
#rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

# Detect OS and set generator
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    # Windows with MSYS2
    GENERATOR="MSYS Makefiles"
    EXTRA_OPTS="-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    GENERATOR="Unix Makefiles"
    # Check for Apple Clang vs regular Clang
    if which clang++ > /dev/null; then
        EXTRA_OPTS="-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++"
    fi
else
    # Linux/Unix
    #GENERATOR="Unix Makefiles"
    GENERATOR="Ninja"
    # Allow selection of compiler
    if [[ "$2" == "gcc" ]]; then
        EXTRA_OPTS="-DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++"
    elif [[ "$2" == "clang" ]]; then
        EXTRA_OPTS="-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++"
    elif [[ "$2" == "icc" ]]; then
        EXTRA_OPTS="-DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc"
    fi
fi

# Configure CMake
cmake .. -G "${GENERATOR}" \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DTLV_BUILD_SHARED=ON \
    -DTLV_BUILD_STATIC=ON \
    -DTLV_BUILD_EXAMPLES=ON \
    -DTLV_WARNINGS_AS_ERRORS=ON \
    -DTLV_ENABLE_LTO=ON \
    -DTLV_ENABLE_SANITIZERS=ON \
    ${EXTRA_OPTS}

# Build
cmake --build . --config ${BUILD_TYPE} --parallel $(nproc)

echo "Build complete in ${BUILD_DIR}/"
