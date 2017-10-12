#include "cmd.h"

int terminal_run()
{
	for (;;)
	{
		printf("%c%d", getchar(), clock_rtc_read());
	} 
}
