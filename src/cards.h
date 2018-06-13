#ifndef CARDS_H
#define CARDS_H

#include <stdlib.h>
#include <time.h>

struct card {
	int interval;
	time_t revday;
	const char *front;
	const char *back;
};

struct deck {
	int cap;
	int size;
	struct card *cards;
};

struct deck *deckptr;

void initdeck(int cap);
void deldeck();
void addcard(struct card *card);

#endif
