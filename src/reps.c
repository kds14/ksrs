#include "reps.h"
#include <stdio.h>

struct node {
	struct card *val;
	struct node *prev;
	struct node *next;
};

struct node *next_card;

int queue_count = 0;

void add_rep(struct card *card) {
	struct node *n = malloc(sizeof(struct node));
	n->val = card;
	if (next_card == 0) {
		n->prev = n;
		n->next = n;
		next_card = n;
	} else {
		struct node *rear = next_card->prev;
		next_card->prev = n;
		rear->next = n;
		n->prev = rear;
		n->next = next_card;
	}
	queue_count++;
}

struct card *next_rep() {
	if (queue_count == 0) {
		return 0;
	}
	struct card *result = next_card->val;
	if (next_card->prev == next_card) {
		free(next_card);
	} else {
		struct node *rear = next_card->prev;
		rear->next = next_card->next;
		free(next_card);
		next_card = rear->next;
	}
	queue_count--;
	return result;
}
