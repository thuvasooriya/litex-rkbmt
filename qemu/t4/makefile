# SHELL = /bin/sh

CROSS_COMPILE ?= riscv64-unknown-elf-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
GDB = $(CROSS_COMPILE)gdb

# target configuration
TARGET = kernel
ARCH = rv64imafdc
ABI = lp64
OPTIMIZATION_LEVEL = 0

# directory configuration
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

CFLAGS = -march=$(ARCH) -mabi=$(ABI) -mcmodel=medany -O$(OPTIMIZATION_LEVEL) 
CFLAGS += -Wall -Wextra
CFLAGS += -nostartfiles
CFLAGS += -ffreestanding -nostdlib -nostdinc -static
CFLAGS += -g -fno-omit-frame-pointer # Add debug info
# CFLAGS += -fno-common -ffunction-sections -fdata-sections

# assembler flags
ASFLAGS = -march=$(ARCH) -mabi=$(ABI) -g

# linker flags
LINKER_SCRIPT = linker.ld
LDFLAGS = -T $(LINKER_SCRIPT) 
# LDFLAGS += --gc-sections
LDFLAGS += --no-warn-rwx-segments

SRCS_C = $(wildcard $(SRC_DIR)/*.c)
SRCS_ASM = $(wildcard $(SRC_DIR)/*.S)
OBJS = $(SRCS_C:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) $(SRCS_ASM:$(SRC_DIR)/%.S=$(OBJ_DIR)/%.o)


# QEMU configuration
QEMU = qemu-system-riscv64
QEMU_MACHINE = virt
QEMU_CPU = rv64
QEMU_MEM = 128M
QEMU_ARGS = -machine $(QEMU_MACHINE) 
QEMU_ARGS += -bios none
QEMU_ARGS += -cpu $(QEMU_CPU)
QEMU_ARGS += -m $(QEMU_MEM)
QEMU_ARGS += -nographic
QEMU_ARGS += -display none
# QEMU_ARGS += -serial stdio
QEMU_ARGS += -serial mon:stdio
# QEMU_ARGS += -s
QEMU_DEBUG_ARGS = -S -gdb tcp::1234

# default target
all: clean directories $(BIN_DIR)/$(TARGET).bin $(BIN_DIR)/$(TARGET).lst

# create output directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# compile c source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# compile assembly source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S
	@echo "AS $<"
	@$(AS) $(ASFLAGS) -c $< -o $@

# link object files
$(BIN_DIR)/$(TARGET).elf: $(OBJS)
	@echo "LD $@"
	@$(LD) $(LDFLAGS) $(OBJS) -o $@

# create binary image
# TODO: objcopy flags -O
$(BIN_DIR)/$(TARGET).bin: $(BIN_DIR)/$(TARGET).elf
	@echo "OBJCOPY $@"
	@$(OBJCOPY) -I binary $< $@

# create disassembly listing
$(BIN_DIR)/$(TARGET).lst: $(BIN_DIR)/$(TARGET).elf
	@echo "OBJDUMP $@"
	@$(OBJDUMP) -d $< > $@

# run in QEMU
run: all
	@echo "running in QEMU..."
	@$(QEMU) $(QEMU_ARGS) -kernel $(BIN_DIR)/$(TARGET).bin

# debug with QEMU and GDB
# terminator -e "riscv64-linux-gnu-gdb -x debug.txt" --new-tab
debug: all
	@echo "starting QEMU in debug mode. connect with GDB using: $(GDB) -ex 'target remote localhost:1234' $(BIN_DIR)/$(TARGET).elf"
	@$(QEMU) $(QEMU_ARGS) $(QEMU_DEBUG_ARGS) -kernel $(BIN_DIR)/$(TARGET).bin

gdb: .gdbinit
	@echo "starting GDB..."
	@$(GDB) -x .gdbinit $(BIN_DIR)/$(TARGET).elf

ugdb: .gdbinit
	@echo "starting UGDB..."
	@ugdb --gdb $(GDB) -x .gdbinit $(BIN_DIR)/$(TARGET).elf

gdbgui: .gdbinit
	@echo "starting GDBGUI..."
	@gdbgui -g '$(GDB) -x .gdbinit $(BIN_DIR)/$(TARGET).elf'

# clean build artifacts
clean:
	@echo "cleaning..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# dump symbols for debugging
symbols: $(BIN_DIR)/$(TARGET).elf
	@$(CROSS_COMPILE)nm -n $< | sort > $(BIN_DIR)/$(TARGET).sym

.PHONY: all directories run debug clean symbols
