#!/usr/bin/env bash
# Builds oracles.nro for the Nintendo Switch (libnx homebrew) with devkitPro.
# One-time setup (WSL/Linux with devkitPro pacman installed):
#   sudo dkp-pacman -S --needed switch-dev switch-cmake switch-pkg-config switch-mesa switch-libdrm_nouveau
#   sudo apt install ninja-build git
# Optional save sync: add switch-curl to the line above and run  ./build_switch.sh -DORACLES_SWITCH_SYNC=ON
set -euo pipefail
: "${DEVKITPRO:=/opt/devkitpro}"
export DEVKITPRO
cd "$(dirname "$0")"
cmake -S . -B build-switch -G Ninja -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/Switch.cmake" "$@"
cmake --build build-switch --target oracles-nx_nro
echo
echo "Done: build-switch/oracles.nro"
echo "Copy it to sdmc:/switch/oracles/ and your USA ROMs to sdmc:/switch/oracles/roms/"
