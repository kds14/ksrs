#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

#include "cards.h"
#include "reps.h"
#include "ui.h"

struct app_state *aps;

char getch()
{
	if (aps->getc_s == REG) {
		return getchar();
	}
	struct termios old, new;

	/* Temporarily changing terminal to read single chars without enter */
	tcgetattr(0, &old);
	new = old;
	new.c_lflag &= ~ICANON;
	new.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &new);

	char ch = getchar();

	tcsetattr(0, TCSANOW, &old);

	return ch;
}


/*
 * Spaced repition software
 */
int main(int argc, char **argv)
{
	aps = calloc(1, sizeof(struct app_state));
	aps->in_s = MAIN;
	aps->card_s = FRONT;
	aps->add_s = FRONT;
	aps->getc_s = SING;

	int deck = 1;
	int df = 0;

	for (int i = 0; i < argc; i++) {
		if (deck == 0) {
			deck_path = argv[i];
			df = 1;
		}
		deck = strcmp(argv[i], "-d");
	}

	if (loaded_deck(df))
		exit(0);

	while (1) {
		char c = getch();
		switch (aps->in_s) {
			case MAIN:
				handle_main(c, aps);
				break;
			case REPS:
				handle_reps(c, aps);
				break;
			case ADD:
				if (handle_add(c, aps)) {
					aps->getc_s = SING;
				}
				break;
		}
	}

	free(aps);
	return 0;
}
