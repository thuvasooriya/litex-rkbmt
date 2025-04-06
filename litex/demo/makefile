BUILD_DIR?=build

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

OBJECTS   = helloc.o calc.o crt0.o main.o
CFLAGS += -O0
CFLAGS += -g -v
# CFLAGS += -D__riscv_flen

all: demo.bin

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -D $< > $(<:.elf=.lst) # full disassembly
	# $(OBJDUMP) -S $< > $(<:.elf=.S)   # source-annotated disassembly
ifneq ($(OS),Windows_NT)
	chmod -x $@
endif

vpath %.a $(PACKAGES:%=../%)

demo.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -T linker.ld -N -o $@ \
		$(OBJECTS) \
		$(PACKAGES:%=-L$(BUILD_DIR)/software/%) \
		-Wl,--whole-archive \
		-Wl,--gc-sections \
		-Wl,-Map,$@.map \
		$(LIBS:lib%=-l%)

ifneq ($(OS),Windows_NT)
	chmod -x $@
endif

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

donut.o: CFLAGS   += -w

VPATH = $(BIOS_DIRECTORY):$(BIOS_DIRECTORY)/cmds:$(CPU_DIRECTORY)


%.o: %.cpp
	$(compilexx)

%.o: %.c
	$(compile)

%.o: %.S
	$(assemble)

clean:
	$(RM) $(OBJECTS) demo.elf demo.bin demo.lst demo.dis .*~ *~

.PHONY: all clean
