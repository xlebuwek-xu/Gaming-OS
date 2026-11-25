CC = x86_64-elf-gcc
LD = x86_64-elf-ld

BUILD_DIR = build

CFLAGS = -Ikernel/include \
	 -std=gnu11 \
         -ffreestanding \
         -fno-stack-protector \
         -fno-pic \
         -mno-red-zone \
         -mcmodel=kernel \
         -Wall \
         -Wextra \
         -O2

LDFLAGS = -nostdlib \
          -z max-page-size=0x1000 \
          -T linker.ld

KERNEL_SOURCES = kernel/core/kernel.c
KERNEL_OBJECTS = $(BUILD_DIR)/kernel.o
KERNEL = $(BUILD_DIR)/kernel.elf

.PHONY: all clean

all: $(KERNEL)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/kernel.o: $(KERNEL_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(KERNEL_OBJECTS) linker.ld
	$(LD) $(LDFLAGS) $(KERNEL_OBJECTS) -o $@

clean:
	rm -rf $(BUILD_DIR)
