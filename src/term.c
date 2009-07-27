#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>

struct winsize *get_winsize(char);
int get_width();
int get_height();
void resize_handler(int);

struct winsize *get_winsize(char resized)
{
	static struct winsize *ws = NULL;

	if (resized == 0 && ws != NULL) return ws;

	if (ws == NULL) {
		ws = (struct winsize *)malloc(sizeof(struct winsize));
	}

	ioctl(fileno(stdout), TIOCGWINSZ, ws);

	return ws;
}

int get_width()
{
	struct winsize *ws = get_winsize(0);
	return ws->ws_col;
}

int get_height()
{
	struct winsize *ws = get_winsize(0);
	return ws->ws_row;
}

void resize_handler(int sig)
{
	get_winsize(1);

	signal(SIGWINCH, resize_handler);
}

