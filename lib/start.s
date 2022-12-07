.globl _start
_start:
	ldr x0, [sp]
	add x1, sp, #8
	b init
