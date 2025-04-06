# primary functions:
# - stack allocation and initializing stack pointer
# - jumping to main

.section .text
.section .text.init, "ax", @progbits # executable code section

.global _start

_start:
	la    sp, __stack_top    # Load the stack pointer
	mv    a0, sp
	csrr  t0, mstatus
	li    t1, 8192      # 1 << 13 = 8192
	or    t0, t1, t0
	csrw  mstatus, t0
	csrwi fcsr, 0
	add   s0, sp, zero      # Set the frame pointer
	jal   zero, main        # Run main entry point - no argc

loop:
	j loop              # Spin forever in case main returns

	.section .data
	.space   1024*8             # allocate 8K of memory to serve as initial stack
	.align   16                 # Smallest stack allocation is 16 bytes, so align accordingly

	__stack_top:              # The stack grows downward according the Risc-V ABI

