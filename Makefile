# Makefile — Builds the kernel ELF and runs it under QEMU
# --------------------------------------------------------
# CROSS allows users to override toolchain prefix.
CROSS ?= riscv64-unknown-elf

# Compiler, linker, objcopy based on prefix
CC := $(CROSS)-gcc
LD := $(CROSS)-ld
OBJCOPY := $(CROSS)-objcopy

# Object files that make up the kernel
OBJS := src/kernel.o src/console.o src/fs.o src/scheduler.o src/intr.o src/boot.o

# Basic flags:
# -march/abi       = generate RISC-V 64-bit instructions
# -nostdlib        = no C standard library (bare metal)
# -fno-builtin     = prevent GCC from assuming libc exists
# -O2, -g          = optimize but keep symbols for debug
CFLAGS := -march=rv64imac -mabi=lp64 -O2 -g -nostdlib -fno-builtin -I.

# Linker flags use the provided linker script
LDFLAGS := -T linker.ld

.PHONY: all clean run

# Build kernel
all: build/kernel.elf

# Directory for build artifacts
build:
	mkdir -p build

# Link ELF
build/kernel.elf: $(OBJS) linker.ld | build
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Pattern rules for compiling .c and .S
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

src/%.o: src/%.S
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build *.o

# Run via QEMU Virt Platform
run: build/kernel.elf
	qemu-system-riscv64 -machine virt -nographic -m 256M -kernel build/kernel.elf
