#!/usr/bin/env bash
set -euo pipefail

##############################################################################
# CONFIG
##############################################################################
BUILD_DIR="build"                   # out-of-tree build folder
SRC_DIR="$(dirname "$0")"           # path to CMakeLists.txt is now the script's dir
RUN_TESTS=false                     # Flag to control test execution

# Parse arguments
if [[ "$#" -gt 0 && ("$1" == "--test" || "$1" == "--tests") ]]; then
    RUN_TESTS=true
fi

##############################################################################
# Optional: wipe the old build (comment out for incremental rebuilds)
##############################################################################
echo "🧹 Wiping old build directory..."
rm -rf "$BUILD_DIR"

##############################################################################
# 1. configure
##############################################################################
echo "⚙️ Configuring project..."
cmake -S "$SRC_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release

##############################################################################
# 2. build (parallel if we can detect CPU cores)
##############################################################################
echo "🛠️ Building project..."
case "$(uname -s)" in
  Linux*)  JOBS=$(nproc) ;;
  Darwin*) JOBS=$(sysctl -n hw.ncpu) ;;
  *)       JOBS=1 ;;                      # Windows MSVC/NMake handles /MP itself
esac

cmake --build "$BUILD_DIR" --parallel "$JOBS"

##############################################################################
# 3. locate & run the executable / tests
##############################################################################
if $RUN_TESTS; then
    echo "▶️ Running tests..."
    test_exe_path=""
    if [[ -x "$BUILD_DIR/GameTests" ]]; then
        test_exe_path="$BUILD_DIR/GameTests"
    elif [[ -x "$BUILD_DIR/Release/GameTests.exe" ]]; then # For Windows MSVC
        test_exe_path="$BUILD_DIR/Release/GameTests.exe"
    elif [[ -x "$BUILD_DIR/GameTests.exe" ]]; then # For Windows Ninja/MinGW
        test_exe_path="$BUILD_DIR/GameTests.exe"
    fi

    if [[ -z "$test_exe_path" ]]; then
        echo "❌ Could not find the GameTests executable after build."
        exit 1
    fi

    exec "$test_exe_path"
else
    exe_path=""
    # Single-config generators (Ninja, Makefiles) place it right in $BUILD_DIR
    if [[ -x "$BUILD_DIR/Chess" ]]; then
        exe_path="$BUILD_DIR/Chess"
    elif [[ -x "$BUILD_DIR/Chess.exe" ]]; then
        exe_path="$BUILD_DIR/Chess.exe"
    else
        # Multi-config (Visual Studio, Xcode): default to Release
        if [[ -x "$BUILD_DIR/Release/Chess.exe" ]]; then
            exe_path="$BUILD_DIR/Release/Chess.exe"
        elif [[ -x "$BUILD_DIR/Release/Chess" ]]; then
            exe_path="$BUILD_DIR/Release/Chess"
        fi
    fi

    if [[ -z "$exe_path" ]]; then
        echo "❌  Could not find the Chess executable after build."
        exit 1
    fi

    echo "▶️ Running $exe_path ..."
    exec "$exe_path"
fi

echo "✅ Build and run completed successfully."