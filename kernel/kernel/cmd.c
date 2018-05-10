#include "cmd.h"

int terminal_run()
{
	int previous = -1;
	int lastTime = -1;
	int delay = 1;
	for (;;)
	{
		int c = getcharRaw();
		int t = clock_rtc_read();

		if (c == 0)
		{
			previous = -1;
			lastTime = -1;
			continue;
		}

		if (previous == c)
		{
			if (lastTime < t - 1)
			{
				type(c);
				lastTime = t;
			}
		}
		else
		{
			type(c);
			previous = c;
			lastTime = t;
		}

		//printf("%c%i", getchar(), clock_rtc_read());
	}
}

void type(int c)
{
	if (c == 8)
	{
		printf("\b");
	}
	else
	{
		printf("%c", c);
	}
}
