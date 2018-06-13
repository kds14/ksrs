#include "cards.h"

void initdeck(int cap) {
	deckptr = malloc(sizeof(struct deck));
	deckptr->cards = malloc(cap * sizeof(struct card *));
	deckptr->cap = cap;
	deckptr->size = 0;
}

void deldeck() {
	free(deckptr->cards);
	free(deckptr);
}

void addcard(struct card *card) {
	struct card c = *card;
	free(card);

	if (deckptr->size == deckptr->cap) {
		deckptr->cap *= 2;
		deckptr->cards = malloc(deckptr->cap * sizeof(struct card *));
	}

	deckptr->cards[deckptr->size++] = c;
}
