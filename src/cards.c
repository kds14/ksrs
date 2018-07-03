#include <string.h>
#include <stdio.h>

#include "cards.h"
#include "reps.h"

enum next_read { FCOUNT, BCOUNT, INTSUM, REVDAY };

void init_deck(int cap)
{
	deckptr = calloc(1, sizeof(struct deck));
	deckptr->cards = calloc(cap, sizeof(struct card *));
	deckptr->cap = cap;
	deckptr->size = 0;
}

void create_card(char *front, char *back)
{
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
void add_card(struct card *card)
{

	if (deckptr->size + 1 >= deckptr->cap) {
		deckptr->cap *= 2;
		struct card **temp = calloc(deckptr->cap, sizeof(struct card *));
		memcpy(temp, deckptr->cards, deckptr->size * sizeof(struct card *));
		free(deckptr->cards);
		deckptr->cards = temp;
	}
	deckptr->cards[deckptr->size++] = card;
}

void print_deck(struct deck *deck)
{
	for (int i = 0; i < deck->size; i++) {
		printf("Card %d:\n", i);
		print_card(deck->cards[i]);
	}
}

void print_card(struct card *card)
{
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
int read_deck(char *filestr)
{
	FILE *fp = fopen(filestr, "r");
	if (fp == 0) {
		return 1;
	}
	char *buff = (char *) calloc(512, sizeof(char));;
	enum next_read next = FCOUNT;

	init_deck(100);

	char c;
	int count = 0;
	char *num_buff = 0;
	struct card *card;
	int succ = 1;

	int fcount = 0;
	int bcount = 0;

	while ((c = fgetc(fp)) != EOF && succ) {
		int i = 0;
		unsigned long int l = 0;
		if (c == ',' || c == '\n') {
			switch (next++) {
				case FCOUNT:
					card = calloc(1, sizeof(struct card));
					card->front = 0;
					card->back = 0;
					card->revday = 0;
					card->intsum = 0;

					if (count < 1) {
						printf("%s", "Error loading card\n");
						return 1;
					}
					num_buff = calloc(count + 1, sizeof(char));
					memcpy(num_buff, buff, count * sizeof(char));
					fcount = 0;
					bcount = 0;
					sscanf(num_buff, "%5d", &fcount);
					free(num_buff);
					num_buff = 0;
					break;
				case BCOUNT:
					if (count < 1) {
						printf("%s", "Error loading card\n");
						return 1;
					}
					num_buff = calloc(count + 1, sizeof(char));
					memcpy(num_buff, buff, count * sizeof(char));
					sscanf(num_buff, "%5d", &bcount);
					free(num_buff);
					num_buff = 0;
					if (bcount == 0 || fcount == 0) {
						succ = 0;
						break;
					}
					card->front = calloc(fcount + 1, sizeof(char));
					for (int i = 0; i < fcount; i++) {
						c = fgetc(fp);
						if (c == EOF) {
							succ = 0;
						}
						card->front[i] = c;
					}
					fgetc(fp);
					card->back = calloc(bcount + 1, sizeof(char));
					for (int i = 0; i < bcount; i++) {
						c = fgetc(fp);
						if (c == EOF) {
							succ = 0;
						}
						card->back[i] = c;
					}
					c = fgetc(fp);
					break;
				case INTSUM:
					if (count > 0) {
						num_buff = calloc(count+ 1, sizeof(char));
						memcpy(num_buff, buff, count * sizeof(char));
						sscanf(num_buff, "%5d", &i);
						free(num_buff);
						num_buff = 0;
						card->intsum = i;
						i = 0;
					}
					break;
				case REVDAY:
					if (count > 0) {
						num_buff = calloc(count + 1, sizeof(char));
						memcpy(num_buff, buff, count * sizeof(char));
						sscanf(num_buff, "%lu", &l);
						free(num_buff);
						num_buff = 0;
						card->revday = (time_t)l;
						l = 0;
					}
					break;
			}
			if (c == '\n') {
				if (card->revday == 0) {
					card->revday = time(0);
				}
				if (card->front == 0 || card->back == 0) {
					printf("%s\n", "Failed to add card. Must have a front and back.");
				}
				else {
					add_card(card);
				}
				next = FCOUNT;
			}
			free(buff);
			buff = calloc(512, sizeof(char));;
			count = 0;
		} else {
			buff[count++] = c;
		}
	}

	free(buff);

	for (int i = 0; i < deckptr->size; i++) {
		add_rep_if_due(deckptr->cards[i]);
	}

	fclose(fp);
	return !succ;
}

int write_deck(char *filestr, struct deck *deck)
{
	//+
	FILE *fp = fopen(filestr, "w+");
	if (fp == 0) {
		return 1;
	}
	for (int i = 0; i < deck->size; i++) {
		struct card *card = deck->cards[i];
		fprintf(fp, "%lu,%lu,%s,%s,%d,%lu\n", strlen(card->front), strlen(card->back), card->front, card->back, card->intsum, (unsigned long)card->revday);
	}
	printf("%s", "Deck saved\n");
	fclose(fp);
	return 0;
}
