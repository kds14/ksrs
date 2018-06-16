#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include "cards.h"
#include "reps.h"

char getch() {
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

int main() {
	printf("%s", "Press any key\n");

	read_deck("../ksrs/build/deck.txt");

	while (1) {
		char c = getch();
		printf("%c\n", c);
	}

	return 0;
}
