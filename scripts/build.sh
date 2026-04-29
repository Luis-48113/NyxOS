#!/bin/bash
set -e

echo "=== NyxOS Build Helper ==="

# Check for i686-elf-gcc
if ! command -v i686-elf-gcc &> /dev/null; then
    echo "ERROR: i686-elf-gcc not found!"
    echo "Install: sudo apt-get install gcc-i686-linux-gnu binutils-i686-linux-gnu"
    exit 1
fi

echo "[1] Building kernel..."
cd /workspaces/NYXOS
make clean
make

echo "[2] Creating ISO..."
make iso

echo "[3] Build complete!"
echo "    Run with: make run"
echo "    Or: qemu-system-i386 -cdrom build/nyxos.iso -m 256M"
