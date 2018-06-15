#include <string.h>
#include <stdio.h>

#include "cards.h"

void init_deck(int cap) {
	deckptr = malloc(sizeof(struct deck));
	deckptr->cards = malloc(cap * sizeof(struct card *));
	deckptr->cap = cap;
	deckptr->size = 0;
}

void del_deck() {
	for (int i = 0; i < deckptr->size; i++) {
		free(deckptr->cards[i].front);
		free(deckptr->cards[i].back);
	}
	free(deckptr->cards);
	free(deckptr);
}

void add_card(struct card *card) {
	struct card c = *card;

	if (deckptr->size >= deckptr->cap) {
		struct card *temp = malloc(deckptr->cap * sizeof(struct card *));
		memcpy(temp, deckptr->cards, sizeof(struct card) * deckptr->size);
		deckptr->cap *= 2;
		free(deckptr->cards);
		deckptr->cards = malloc(deckptr->cap * sizeof(struct card *));
		memcpy(deckptr->cards, temp, sizeof(struct card) * deckptr->size);
		free(temp);
	}

	deckptr->cards[deckptr->size++] = c;
}

void print_deck(struct deck *deck) {
	for (int i = 0; i < deck->size; i++) {
		printf("Card %d:\n", i);
		printf("Front: %s\n", deck->cards[i].front);
		printf("Back: %s\n", deck->cards[i].back);
		printf("Interval: %d\n", deck->cards[i].interval);
		printf("RevDay: %lu\n", (unsigned long)deck->cards[i].revday);
	}
}

void read_deck(char *filestr) {
	FILE *fp = fopen(filestr, "r");
	char buff[512];
	memset(buff, 0, sizeof(buff));
	enum next_read next = FRONT;
	struct card *card;

	init_deck(100);

	char c;
	int count = 0;
	while ((c = fgetc(fp)) != EOF) {
		int i = 0;
		unsigned long int l = 0;
		if (c == '\t' || c == '\n') {
			switch (next++) {
				case FRONT:
					card = malloc(sizeof(card));
					card->front = malloc(sizeof(char) * count);
					memcpy(card->front, buff, count*sizeof(char));
					break;
				case BACK:
					card->back = malloc(sizeof(char) * count);
					memcpy(card->back, buff, count * sizeof(char));
					break;
				case INT:
					sscanf(buff, "%d", &i);
					card->interval = i;
					i = 0;
					break;
				case REVDAY:
					sscanf(buff, "%lu", &l);
					card->revday = (time_t)l;
					l = 0;
					break;
			}
			if (c == '\n') {
				add_card(card);
				next = FRONT;
			}
			memset(buff, 0, sizeof(buff));
			count = 0;
		} else {
			buff[count++] = c;
		}
	}
}
