#include <string.h>
#include <stdio.h>

#include "cards.h"
#include "reps.h"

enum next_read { FCOUNT, BCOUNT, INTSUM, REVDAY };

void set_current_time(struct card *card) {
	time_t ct;
	time(&ct);
	card->revday = localtime(&ct);
	card->revday->tm_sec = 0;
	card->revday->tm_min = 0;
	card->revday->tm_hour = 0;
}

char *write_time(struct card *card) {
	char *buff = calloc(9, sizeof(char));
	sprintf(buff, "%02d%02d%04d", card->revday->tm_mon + 1,
			card->revday->tm_mday, card->revday->tm_year + 1900);
	return buff;
}

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
	set_current_time(card);
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

	char *time = write_time(card);
	printf("RevDay: %s \n", time);
	free(time);
}

/*
 * Reads all card data from a deck file and adds them to the deck.
 */
int read_deck(char *filestr)
{
	int buff_size = 2048;
	FILE *fp = fopen(filestr, "r");
	if (fp == 0) {
		return 1;
	}
	char *buff = (char *) calloc(buff_size, sizeof(char));;
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
		if (count - 2 == buff_size) {
			printf ("%s\n", "Exceeded char buffer input size");
			return 1;
		}
		if (c != ',' && c != '\n') {
			buff[count++] = c;
			continue;
		}
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
				if (count == 8) {
					set_current_time(card);
					num_buff = calloc(5, sizeof(char));
					memcpy(num_buff, buff, 2 * sizeof(char));
					sscanf(num_buff, "%d", &i);
					card->revday->tm_mon = i - 1;
					memcpy(num_buff, buff + 2, 2 * sizeof(char));
					sscanf(num_buff, "%d", &i);
					card->revday->tm_mday = i;
					memcpy(num_buff, buff + 4, 4 * sizeof(char));
					sscanf(num_buff, "%d", &i);
					card->revday->tm_year = i - 1900;
					free(num_buff);
					num_buff = 0;
					i = 0;
				}
				break;
		}

		if (c == '\n') {
			if (card->revday == 0) {
				set_current_time(card);
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
		buff = calloc(buff_size, sizeof(char));;
		count = 0;
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
		char *time = write_time(card);
		fprintf(fp, "%lu,%lu,%s,%s,%d,%s\n", strlen(card->front), strlen(card->back), card->front, card->back, card->intsum, time);
		free(time);
	}
	printf("%s", "Deck saved\n");
	fclose(fp);
	return 0;
}
