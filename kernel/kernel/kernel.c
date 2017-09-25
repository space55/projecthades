#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>

#if defined(__i386__)
#include "../arch/i386/idt.h"
#include "../arch/i386/pic.h"
#include "../arch/i386/io_hid.h"

#endif

void kernel_early(void)
{
	terminal_initialize();
}

void kernel_main(void)
{
	printf("Hello, kernel World!\n");

#if defined(__i386__)
	printf("idt_initialize\n");
	idt_initialize();
	pic_initialize();
#endif

	for (;;)
	{
		printf("%c\n", getchar());
	}
}
