#!/bin/bash
# Build 7-Zip Zstd with Wine/Linux support
# Cross-compiles Windows binary using MinGW-w64
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# Default options
WINE_LINUX=${WINE_LINUX:-ON}
BUILD_TYPE=${BUILD_TYPE:-Release}
JOBS=${JOBS:-$(nproc 2>/dev/null || echo 4)}
CLEAN=${CLEAN:-0}

print_usage() {
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  --no-wine       Disable Wine/Linux integration (Z7_WINE_LINUX=OFF)"
    echo "  --debug         Debug build"
    echo "  --clean         Clean build directory first"
    echo "  --help          Show this help"
    echo ""
    echo "Environment:"
    echo "  WINE_LINUX=OFF  Disable Wine support"
    echo "  BUILD_TYPE=Debug"
    echo "  JOBS=N          Parallel jobs (default: nproc)"
    echo ""
    echo "Prerequisites:"
    echo "  sudo apt install g++-mingw-w64-x86-64 cmake make"
}

for arg in "$@"; do
    case "$arg" in
        --no-wine)  WINE_LINUX=OFF ;;
        --debug)    BUILD_TYPE=Debug ;;
        --clean)    CLEAN=1 ;;
        --help)     print_usage; exit 0 ;;
        *)          echo "Unknown option: $arg"; print_usage; exit 1 ;;
    esac
done

# Check toolchain
MINGW_CXX="x86_64-w64-mingw32-g++"
MINGW_CC="x86_64-w64-mingw32-gcc"
for cmd in "$MINGW_CXX" "$MINGW_CC"; do
    if ! command -v "$cmd" &>/dev/null; then
        echo "ERROR: Missing cross-compiler: $cmd" >&2
        echo "Install: sudo apt install g++-mingw-w64-x86-64" >&2
        exit 1
    fi
done

# Build directory
BUILD_DIR="$SCRIPT_DIR/build"
if [ "$CLEAN" = "1" ]; then
    echo "=== Cleaning build directory ==="
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"

echo "=== 7-Zip Zstd Wine Cross-compilation ==="
echo "  Target:         x86_64-w64-mingw32"
echo "  Build type:     $BUILD_TYPE"
echo "  Z7_WINE_LINUX:  $WINE_LINUX"
echo "  Build dir:      $BUILD_DIR"
echo "  Jobs:           $JOBS"
echo ""

cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" \
    -DCMAKE_TOOLCHAIN_FILE="$SCRIPT_DIR/cmake/toolchain-mingw-w64.cmake" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DZ7_WINE_LINUX="$WINE_LINUX" \
    -DCMAKE_CXX_FLAGS="-Wno-unknown-pragmas" \
    -DCMAKE_C_FLAGS="-Wno-unknown-pragmas"

echo ""
echo "=== Building ==="
cmake --build "$BUILD_DIR" -j "$JOBS"

echo ""
echo "=== Build complete ==="
TARGET="$BUILD_DIR/7zFM/7zFM.exe"
if [ -f "$TARGET" ]; then
    SIZE=$(stat -c%s "$TARGET" 2>/dev/null)
    echo "✓ 7zFM.exe: $TARGET ($SIZE bytes)"
    echo ""
    echo "To run: wine $TARGET"
else
    echo "Warning: 7zFM.exe not found at $TARGET"
    echo "Check $BUILD_DIR for output files."
fi
