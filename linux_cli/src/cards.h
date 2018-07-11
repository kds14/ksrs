#ifndef CARDS_H
#define CARDS_H

#include <stdlib.h>
#include <time.h>

struct card {
	int intsum;
	struct tm *revday;
	char *front;
	char *back;
};

struct deck {
	int cap;
	int size;
	struct card **cards;
};

struct deck *deckptr;
struct card *current;


int	read_deck(char *filestr);
int write_deck(char *filestr, struct deck *deck);
void print_deck(struct deck *deck);
void print_deck();
void print_card(struct card *card);
void set_current_time(struct card *card);

void init_deck(int cap);
void del_deck();
void add_card(struct card *card);
void create_card(char *front, char *back);

char *deck_path;

#endif
