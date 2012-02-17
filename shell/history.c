#include "history.h"

int hist_position(int delta)
{
	static int position = 0;
	position = (position+delta) % MAX_HISTORY_POSITION;
	return position;
}


void hist_store(char *line)
{
	hist_backing_array[hist_position(0)] = strdup(line);
}
char *hist_get()
{
	return strdup(hist_backing_array[hist_position(0)]);
}

void hist_next()
{
	hist_position(1);
}
void hist_back()
{
	hist_position(-1);
}
