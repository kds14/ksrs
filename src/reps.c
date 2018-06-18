#include <time.h>
#include <stdio.h>


#include "reps.h"

struct node {
	struct card *val;
	struct node *prev;
	struct node *next;
};

struct node *next_card;

void add_rep(struct card *card);
int next_interval(int correct, int interval);

int queue_count = 0;

void add_rep_if_due(struct card *card) {
	if (card->revday <= time(0)) {
		add_rep(card);
	}
}

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
	//print_card(next_card->val);
	queue_count++;
}

/*
 * Returns the next card in the queue.
 */
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
		next_card->prev = rear;
	}
	queue_count--;
	return result;
}

int next_interval(int correct, int interval) {
	if (correct < 3) {
		return 1;
	}
	else {
		return 1.3f * interval + 1;
	}
}

/*
 * Changes card data and adds to queue based on if
 * the card was answered correctly or not.
 */
void answer_card(enum answer ans, struct card *card) {
	struct tm *timeptr = localtime(&card->revday);
	if (ans == RIGHT) {
		card->correct++;
		card->interval = next_interval(card->correct, card->interval);
	} else {
		card->interval = 1;
		card->correct = 0;
		add_rep(card);
	}
	timeptr->tm_mday +=  card->interval;
	card->revday = mktime(timeptr);
}
