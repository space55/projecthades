#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>
#include "io.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_getentryat(size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	return terminal_buffer[index];
}

static void terminal_scroll()
{
	uint16_t *buf = terminal_buffer;
	for (size_t y = 1; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			buf[(y - 1) * VGA_WIDTH + x] = buf[y * VGA_WIDTH + x];
		}
	}
	for (size_t x = 0; x < VGA_WIDTH; x++)
	{
		buf[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = make_vgaentry(' ', terminal_color);
	}
}

static void terminal_newline()
{
	terminal_column = 0;
	if (terminal_row + 1 == VGA_HEIGHT)
	{
		terminal_scroll();
	}
	else
	{
		terminal_row++;
	}
}

void terminal_putchar(char c)
{
	if (c == '\n')
	{
		terminal_newline();
	}
	else if (c == '\r')
	{
		terminal_column = 0;
	}
	else if (c == '\b')
	{
		if (terminal_row != 0 || terminal_column != 0) {
			terminal_putentryat(' ', terminal_color, terminal_column - 1, terminal_row);
			if (terminal_column == 0) 
			{
				terminal_row--;
				terminal_column = VGA_WIDTH - 1;
			}
			else 
			{
				terminal_column--;
			}
		}
	}
	else
	{
		if (terminal_column == VGA_WIDTH)
		{
			terminal_newline();
		}
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		terminal_column++;
	}
	update_cursor(terminal_column, terminal_row);
}

void terminal_write(const char *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char *data)
{
	terminal_write(data, strlen(data));
}