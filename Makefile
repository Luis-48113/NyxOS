CC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-ld
OBJCOPY = i686-elf-objcopy
QEMU = qemu-system-i386

CFLAGS = -std=c99 -ffreestanding -Wall -Wextra -I. -Iinclude -nostdlib -fno-builtin
ASFLAGS = --32
LDFLAGS = -T linker.ld -nostdlib -Wl,--entry=_entry

KERNEL_SOURCES = \
	kernel/arch/x86/entry.asm \
	kernel/arch/x86/idt.c \
	kernel/arch/x86/irq.c \
	kernel/cpu/cpu.c \
	kernel/drivers/fb.c \
	kernel/drivers/graphics.c \
	kernel/drivers/screen.c \
	kernel/drivers/keyboard.c \
	kernel/drivers/timer.c \
	kernel/mm/memory.c \
	kernel/fs/fs.c \
	kernel/sched/sched.c \
	shell/shell.c \
	shell/commands.c \
	apps/help.c \
	apps/echo.c \
	apps/clear.c \
	apps/osinfo.c \
	apps/shutdown.c \
	apps/snake.c \
	lib/string.c \
	kernel/kernel.c

KERNEL_OBJECTS = $(KERNEL_SOURCES:.asm=.o)
KERNEL_OBJECTS := $(KERNEL_OBJECTS:.c=.o)
KERNEL_ELF = build/kernel.elf
ISO_FILE = build/nyxos.iso

all: $(ISO_FILE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL_ELF): $(KERNEL_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

iso: $(KERNEL_ELF)
	mkdir -p iso/boot/grub
	cp $(KERNEL_ELF) iso/boot/
	cp boot/grub/grub.cfg iso/boot/grub/
	grub-mkrescue -o $(ISO_FILE) iso 2>/dev/null || \
		xorriso -as mkisofs -R -b boot/grub/i386-pc/eltorito.img -no-emul-boot iso -o $(ISO_FILE)

$(ISO_FILE): iso

run: $(ISO_FILE)
	$(QEMU) -cdrom $(ISO_FILE) -m 256M

debug: $(ISO_FILE)
	$(QEMU) -cdrom $(ISO_FILE) -m 256M -s -S &

clean:
	rm -f $(KERNEL_OBJECTS) $(KERNEL_ELF) $(ISO_FILE)
	rm -rf iso/

.PHONY: all run debug clean iso
