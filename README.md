# NyxOS v1-STABLE

A minimal 32-bit monolithic x86 operating system with GRUB multiboot and framebuffer graphics support.

## Architecture

- **CPU**: 32-bit x86 protected mode
- **Bootloader**: GRUB multiboot compliant with framebuffer support
- **Graphics**: GRUB framebuffer (800x600 default resolution)
- **Memory**: Custom heap allocator (128KB)
- **Input**: PS/2 keyboard with ASCII and arrow key support
- **Timer**: IRQ0-based system timer
- **Filesystem**: Static in-memory VFS
- **Shell**: Command interpreter with multiple built-in commands

## Features Implemented

### Core Systems
- ✅ Multiboot GRUB bootloader with framebuffer info
- ✅ 32-bit x86 protected mode kernel
- ✅ CPU detection (CPUID-based vendor/brand strings)
- ✅ Custom heap memory management (malloc/free with coalescing)
- ✅ PS/2 keyboard driver with input buffering
- ✅ IRQ-based timer system (100Hz)
- ✅ Static VFS with /readme, /version, /ram files

### Graphics System
- ✅ GRUB framebuffer initialization
- ✅ Pixel plotting and rectangle drawing
- ✅ 8x8 bitmap font text rendering
- ✅ Color support (RGB/RGBA)

### Boot Splash Screen
- ✅ Centered NyxOS ASCII logo
- ✅ CPU information display
- ✅ Loading status messages with timing
- ✅ Automatic transition to shell

### Shell Commands
- ✅ help - Display available commands
- ✅ clear - Clear screen
- ✅ echo - Echo text
- ✅ osinfo - Display system information
- ✅ version - Display version
- ✅ uname - Display system info
- ✅ whoami - Display current user (simulated)
- ✅ ls - List VFS files
- ✅ cat - Display file contents
- ✅ meminfo - Display memory statistics
- ✅ snake - Play Snake game (framebuffer required)
- ✅ shutdown - Shutdown system

### Applications
- ✅ osinfo - System information display
- ✅ snake - Timer-driven Snake game with framebuffer graphics

## Build Requirements

- i686-elf-gcc cross-compiler
- i686-elf-binutils
- make
- GRUB (for ISO creation)

## Building

```bash
# Install cross-compiler (Ubuntu/Debian)
sudo apt-get install gcc-i686-linux-gnu binutils-i686-linux-gnu

# Build the OS
./scripts/build.sh

# Run in QEMU
qemu-system-i386 -cdrom build/nyxos.iso -m 256M
```

## Stability Features

- ✅ Consistent boot in QEMU and VirtualBox
- ✅ Memory corruption testing (repeated malloc/free cycles)
- ✅ Stable framebuffer rendering
- ✅ Reliable input handling under timer load
- ✅ Snake game stability (no crashes or desync)

## Project Structure

```
nyxos/
├── kernel/
│   ├── kernel.c/h          # Main kernel entry
│   ├── arch/x86/           # Architecture-specific code
│   │   ├── entry.asm       # Multiboot entry point
│   │   ├── idt.c/h         # Interrupt descriptor table
│   │   └── irq.c/h         # Interrupt request handling
│   ├── cpu/                # CPU detection
│   ├── drivers/            # Device drivers
│   │   ├── fb.c/h          # Framebuffer graphics
│   │   ├── graphics.c/h    # Graphics abstraction
│   │   ├── screen.c/h      # Text screen (fallback)
│   │   ├── keyboard.c/h    # PS/2 keyboard
│   │   └── timer.c/h       # System timer
│   ├── mm/                 # Memory management
│   ├── fs/                 # Virtual filesystem
│   └── sched/              # Scheduler foundation
├── shell/                  # Command shell
├── apps/                   # User applications
├── lib/                    # Standard library
├── include/                # Header files
├── boot/grub/              # GRUB configuration
├── scripts/                # Build scripts
└── Makefile                # Build configuration
```

## Design Principles

- Monolithic kernel but modular folder structure
- Prioritize stability over advanced features
- Framebuffer as primary display system
- Minimal but complete OS functionality
- Clean separation of concerns
- Well-documented code with consistent APIs