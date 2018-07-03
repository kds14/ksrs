#include <stdio.h>
#include <string.h>

#include "ui.h"
#include "reps.h"

struct card *display_rep()
{
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

int loaded_deck(int df)
{
	if (df) {
		if (read_deck(deck_path)) {
			printf("%s %s\n", "Failed to load deck at path:", deck_path);
			return 1;
		}
		else {
			printf("Reps due: %d\n", queue_count);
			printf("%s\n", "Do reps (Q), Add card (W)");
		}
	} else {
		printf("%s\n", "No deck loaded. Please enter a deck path.");
		return 1;
	}
	return 0;

}

/*
 *	Handles the main input state. It will make changes to the
 *	state pointer.
 */
void handle_main(char c, struct app_state *aps)
{
	if (c == 'Q' || c == 'q') {
		if (queue_count > 0) {
			aps->in_s = REPS;
			current = display_rep();
		}
	} else if (c == 'W' || c == 'w') {
		aps->in_s = ADD;
		aps->add_s = INIT;
		aps->getc_s = REG;
		printf("%s\n", "Input front:");
	}
}

void handle_reps(char c, struct app_state *aps)
{
	if (c == 'Q' || c == 'q' || c == 32) {
		if (aps->card_s == BACK) {
			answer_card(RIGHT, current);
			aps->card_s = FRONT;
			current = display_rep();
			if (!current) {
				aps->in_s = MAIN;
				if (write_deck(deck_path, deckptr)) {
					printf("%s\n", "Failed to write deck.");
				}
			}
		} else {
			display_back_rep(current);
			aps->card_s = BACK;
		}
	} else if (aps->card_s == BACK && (c == 'W' || c == 'w')) {
		answer_card(WRONG, current);
		aps->card_s = FRONT;
		current = display_rep();
	}
}

int input_count = 0;
char *input_buffer = 0;
char *front = 0;
char *back = 0;

/* Cleans up input buffers */
void clean_in_buffs()
{
	if (front) {
		free(front);
		front = 0;
	}
	if (back) {
		free(back);
		back = 0;
	}
	if (input_buffer) {
		free(input_buffer);
		input_buffer = 0;
	}
}

void handle_add(char c, struct app_state *aps)
{
	if (c == 'C' || c == 'c') {
		clean_in_buffs();
	}
	switch (aps->add_s) {
		case INIT:
			clean_in_buffs();
			input_count = 0;
			input_buffer = calloc(1024, sizeof(char));
			aps->add_s = FRONT;
		case FRONT:
			if (c == 10) {
				front = calloc(input_count + 1, sizeof(char));
				memcpy(front, input_buffer, input_count);
				input_count = 0;
				aps->add_s = BACK;
				printf("%s\n", "Input back:");
			} else {
				input_buffer[input_count++] = c;
			}
			break;
		case BACK:
			if (c == 10) {
				back = calloc(input_count + 1, sizeof(char));
				memcpy(back, input_buffer, input_count);
				create_card(front, back);
				write_deck(deck_path, deckptr);
				aps->in_s = MAIN;
				front = 0;
				back = 0;
				free(input_buffer);
				input_buffer = 0;
				printf("Reps due: %d\n", queue_count);
				printf("%s\n", "Do reps (Q), Add card (W)");
			} else {
				input_buffer[input_count++] = c;
			}
			aps->getc_s = SING;
			break;
	}
}
