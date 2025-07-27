#!/usr/bin/env bash
set -euo pipefail

##############################################################################
# CONFIG
##############################################################################
BUILD_DIR="build"                                 # out‐of‐tree build folder
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"       # this script’s directory
SRC_DIR="$SCRIPT_DIR/Chess"                       # CMakeLists.txt lives here now
RUN_TESTS=false                                   # pass --test to run tests

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
  *)       JOBS=1 ;;
esac

cmake --build "$BUILD_DIR" --parallel "$JOBS"

##############################################################################
# 3. locate & run the executable / tests
##############################################################################
if $RUN_TESTS; then
    echo "▶️ Running tests..."
    test_exe="$BUILD_DIR/GameTests"
    if [[ ! -x "$test_exe" ]]; then
      # try Windows MSVC layouts:
      test_exe="$BUILD_DIR/Release/GameTests.exe"
      [[ -x "$test_exe" ]] || test_exe="$BUILD_DIR/GameTests.exe"
    fi
    [[ -x "$test_exe" ]] || { echo "❌ Could not find GameTests"; exit 1; }
    exec "$test_exe"
else
    echo "▶️ Running Chess..."
    exe="$BUILD_DIR/Chess"
    if [[ ! -x "$exe" ]]; then
      # try Windows MSVC layouts:
      exe="$BUILD_DIR/Release/Chess.exe"
      [[ -x "$exe" ]] || exe="$BUILD_DIR/Release/Chess"
    fi
    [[ -x "$exe" ]] || { echo "❌ Could not find Chess executable"; exit 1; }
    exec "$exe"
fi

echo "✅ Build and run completed successfully."
