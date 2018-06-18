#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include "cards.h"
#include "reps.h"

enum state { MAIN, REPS, ADD };

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

struct card *display_rep(int first) {
	struct card *card = next_rep();
	printf("%s\n", "------------");
	if (card != 0) {
		printf("%s\n", card->front);
		printf("%s\n", "------------");
	} else {
		printf("%s\n", "No reps due!");
	}
	return card;
}

void display_back_rep(struct card *card) {
	printf("%s\n", card->back);
}

int main() {
	enum state state = MAIN;
	read_deck("../ksrs/build/deck.txt");
	printf("Reps due: %d\n", queue_count);
	printf("%s\n", "Do reps (Q), Add card (W)");

	/* Front = 0, Back = 1 */
	int card_state = 0;

	struct card *current;

	while (1) {
		char c = getch();
		switch (state) {
			case MAIN:
				if (c == 'Q' || c == 'q') {
					if ((current = display_rep(1))) {
						state = REPS;
					}
				} else if (c == 'W' || c == 'w') {
					printf("%s\n", "Not yet implemented.");
				}
				break;
			case REPS:
				if (c == 'Q' || c == 'q' || c == 32) {
					if (card_state) {
						answer_card(RIGHT, current);
						card_state = 0;
						current = display_rep(0);
						if (!current) {
							state = MAIN;
							continue;
						}
					} else {
						display_back_rep(current);
						card_state = 1;
					}
				} else if (card_state && (c == 'W' || c == 'w')) {
					answer_card(WRONG, current);
					card_state = 0;
					current = display_rep(0);
				}
				break;
			case ADD:
				break;
		}
	}

	return 0;
}
