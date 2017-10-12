#include "cmd.h"

int terminal_run()
{
	for (;;)
	{
		printf("%c%i", getchar(), clock_rtc_read());
	} 
}
