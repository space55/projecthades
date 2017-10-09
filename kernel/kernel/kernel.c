#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>

#include "aarch.h"
#include "cmd.c"

void kernel_early(void)
{
	terminal_initialize();
}

void kernel_main(void)
{
	printf("Welcome to Project Hades\n");

#if defined(__i386__)
	idt_initialize();
	pic_initialize();
#endif

	terminal_run();
}
