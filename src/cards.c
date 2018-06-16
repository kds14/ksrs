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
		print_card(&deck->cards[i]);
	}
}

void print_card(struct card *card) {
	printf("Front: %s\n", card->front);
	printf("Back: %s\n", card->back);
	printf("Interval: %d\n", card->interval);
	printf("Correct: %d\n", card->correct);

	char *buff = malloc(sizeof(char)*100);
	struct tm *timeptr = localtime(&card->revday);
	strftime(buff, 100, "%b %d, %Y %H:%M", timeptr);
	printf("RevDay: %s (%lu)\n", buff, (unsigned long)card->revday);
	free(buff);
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
				case CRCT:
					sscanf(buff, "%d", &i);
					card->correct = i;
					i = 0;
					break;
				case REVDAY:
					sscanf(buff, "%lu", &l);
					card->revday = (time_t)l;
					l = 0;
					break;
			}
			if (c == '\n') {
				if (card->revday == 0) {
					card->revday = time(0);
				}
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
