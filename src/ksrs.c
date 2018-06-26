#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

#include "cards.h"
#include "reps.h"

enum state { MAIN, REPS, ADD };
char *deck_path;
enum get_state { REG, SING };
enum get_state gs = SING;

char getch() {
	if (gs == REG) {
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

/*
 * Spaced repition software
 */
int main(int argc, char **argv) {
	int deck = 1;
	int df = 0;
	for (int i = 0; i < argc; i++) {
		if (deck == 0) {
			deck_path = argv[i];
			df = 1;
		}
		deck = strcmp(argv[i], "-d");
	}

	if (df) {
		if (read_deck(deck_path)) {
			printf("%s %s\n", "Failed to load deck at path:", deck_path);
			exit(0);
		}
		else {
			printf("Reps due: %d\n", queue_count);
			printf("%s\n", "Do reps (Q), Add card (W)");
		}
	} else {
		printf("%s\n", "No deck loaded. Please enter a deck path.");
		exit(0);
	}

	enum state state = MAIN;

	enum card_state { FRONT, BACK };
	enum card_state card_state = FRONT;
	enum card_state add_state = FRONT;

	char *input_buffer = calloc(1024, sizeof(char));
	int input_count = 0;
	char *front;
	char *back;

	int deck_written = 0;

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
					state = ADD;
					add_state = FRONT;
					printf("%s\n", "Add a card (C to cancel)");
					printf("%s\n", "Input front:");
					gs = REG;
					continue;
				}
				break;
			case REPS:
				if (c == 'Q' || c == 'q' || c == 32) {
					if (card_state) {
						answer_card(RIGHT, current);
						card_state = FRONT;
						current = display_rep(0);
						if (!current) {
							state = MAIN;
							if (!deck_written) {
								if (write_deck(deck_path, deckptr)) {
									printf("%s\n", "Failed to write deck.");
								} else {
									deck_written = 1;
								}
							}
							continue;
						}
					} else {
						display_back_rep(current);
						card_state = BACK;
					}
				} else if (card_state && (c == 'W' || c == 'w')) {
					answer_card(WRONG, current);
					card_state = FRONT;
					current = display_rep(0);
				}
				break;
			case ADD:
				if (c == 'C' || c == 'c') {
					memset(input_buffer, 0, 1024 * sizeof(char));
					if (front) {
						free(front);
					}
					if (back) {
						free(back);
					}
					continue;
				}
				switch (add_state) {
					case FRONT:
						if (c == 10) {
							front = calloc(input_count, sizeof(char));
							memcpy(front, input_buffer, input_count);
							input_count = 0;
							add_state = BACK;
							printf("%s\n", "Input back:");
						} else {
							input_buffer[input_count++] = c;
						}
						break;
					case BACK:
						if (c == 10) {
							back = calloc(input_count, sizeof(char));
							memcpy(back, input_buffer, input_count);
							create_card(front, back);
							write_deck(deck_path, deckptr);
							front = 0;
							back = 0;
							input_count = 0;
							state = MAIN;
							printf("Reps due: %d\n", queue_count);
							printf("%s\n", "Do reps (Q), Add card (W)");
						} else {
							input_buffer[input_count++] = c;
						}
						gs = SING;
						break;

				}
				break;
		}
	}


	return 0;
}
