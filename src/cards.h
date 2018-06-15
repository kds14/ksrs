#ifndef CARDS_H
#define CARDS_H

#include <stdlib.h>
#include <time.h>

struct card {
	int interval;
	time_t revday;
	char *front;
	char *back;
};

struct deck {
	int cap;
	int size;
	struct card *cards;
};

struct deck *deckptr;

enum next_read { FRONT, BACK, INT, REVDAY };

void read_deck(char *filestr);
void write_deck(struct deck *deck);
void print_deck(struct deck *deck);
void print_deck();

void init_deck(int cap);
void del_deck();
void add_card(struct card *card);

#endif