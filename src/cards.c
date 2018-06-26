#include <string.h>
#include <stdio.h>

#include "cards.h"
#include "reps.h"

void init_deck(int cap) {
	deckptr = calloc(1, sizeof(struct deck));
	deckptr->cards = calloc(cap, sizeof(struct card *));
	deckptr->cap = cap;
	deckptr->size = 0;
}

// useless atm
void create_card(char *front, char *back) {
	struct card *card = calloc(1, sizeof(struct card));
	card->front = front;
	card->back = back;
	card->intsum = 0;
	card->revday = time(0);
	add_card(card);
	add_rep_if_due(card);
}

/*
 * Adds a card to the current deck and the rep queue if
 * it is due.
 */
void add_card(struct card *card) {

	if (deckptr->size + 1 >= deckptr->cap) {
		deckptr->cap *= 2;
		struct card **temp = calloc(deckptr->cap, sizeof(struct card *));
		memcpy(temp, deckptr->cards, deckptr->size * sizeof(struct card *));
		free(deckptr->cards);
		deckptr->cards = temp;
	}
	deckptr->cards[deckptr->size++] = card;
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
	printf("IntSum: %d\n", card->intsum);

	char *buff = malloc(sizeof(char)*100);
	struct tm *timeptr = localtime(&card->revday);
	strftime(buff, 100, "%b %d, %Y %H:%M", timeptr);
	printf("RevDay: %s (%lu)\n", buff, (unsigned long)card->revday);
	free(buff);
}

/*
 * Reads all card data from a deck file and adds them to the deck.
 */
int read_deck(char *filestr) {
	FILE *fp = fopen(filestr, "r");
	if (fp == 0) {
		return 1;
	}
	char buff[512];
	memset(buff, 0, sizeof(buff));
	enum next_read next = FRONT;
	struct card *card;

	init_deck(100);

	char c;
	int count = 0;
	char *num_buff;
	while ((c = fgetc(fp)) != EOF) {
		int i = 0;
		unsigned long int l = 0;
		if (c == '\t' || c == '\n') {
			/*if ((next == FRONT || next == BACK) && c == '\n') {
				memset(buff, 0, sizeof(buff));
				count = 0;
				continue;
			}*/
			switch (next++) {
				case FRONT:
					card = calloc(1, sizeof(struct card));
					card->back = 0;
					card->revday = 0;
					card->intsum = 0;
					card->front = 0;
					if (count > 0) {
						card->front = malloc(strlen(buff) + 1);
						memcpy(card->front, buff, strlen(buff) + 1);
					}
					break;
				case BACK:
					if (count > 0) {
						card->back = malloc(strlen(buff) + 1);
						memcpy(card->back, buff, strlen(buff) + 1);
					}
					break;
				case INTSUM:
					//num_buff = malloc(count * sizeof(char));
					num_buff = calloc(count, sizeof(char));
					memcpy(num_buff, buff, count * sizeof(char));
					sscanf(num_buff, "%d", &i);
					card->intsum = i;
					i = 0;
					free(num_buff);
					break;
				case REVDAY:
					num_buff = calloc(count, sizeof(char));
					memcpy(num_buff, buff, count * sizeof(char));
					sscanf(num_buff, "%lu", &l);
					card->revday = (time_t)l;
					l = 0;
					free(num_buff);
					break;
			}
			if (c == '\n') {
				if (card->revday == 0) {
					card->revday = time(0);
				}
				if (card->front == 0 || card->back == 0) {
					printf("%s", "Failed to add card. Must have a front and back.");
				}
				else {
					add_card(card);
				}
				next = FRONT;
			}
			memset(buff, 0, sizeof(buff));
			count = 0;
		} else {
			buff[count++] = c;
		}
	}

	for (int i = 0; i < deckptr->size; i++) {
		add_rep_if_due(deckptr->cards[i]);
	}
	fclose(fp);
	return 0;
}

int write_deck(char *filestr, struct deck *deck) {
	FILE *fp = fopen(filestr, "w+");
	if (fp == 0) {
		return 1;
	}
	for (int i = 0; i < deck->size; i++) {
		struct card *card = deck->cards[i];
		fprintf(fp, "%s\t%s\t%d\t%lu\n", card->front, card->back, card->intsum, (unsigned long)card->revday);
	}
	printf("%s", "Deck saved\n");
	fclose(fp);
	return 0;
}
