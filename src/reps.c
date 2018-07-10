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
int	next_interval(int intsum);

int queue_count = 0;

void add_rep_if_due(struct card *card)
{
	if (mktime(card->revday) <= time(0)) {
		add_rep(card);
	}
}

void add_rep(struct card *card)
{
	struct node *n = calloc(1, sizeof(struct node));
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
struct card *next_rep()
{
	if (queue_count == 0) {
		return 0;
	}
	struct card *result = next_card->val;
	if (next_card->prev == next_card) {
		free(next_card);
		next_card = 0;
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

int next_interval(int intsum)
{
	if (intsum < 3) {
		return 1;
	}
	else {
		int i = 1;
		int sum = intsum - 3;
		while (sum > 0) {
			i = 1.3f * i + 1;
			sum -= i;
		}
		return 1.3f * i + 1;
	}
}

/*
 * Changes card data and adds to queue based on if
 * the card was answered correctly or not.
 */
void answer_card(enum answer ans, struct card *card)
{
	int interval = 1;
	if (ans == RIGHT) {
		interval = next_interval(card->intsum);
		card->intsum += interval;
	} else {
		card->intsum = 1;
		add_rep(card);
	}
	set_current_time(card);
	card->revday->tm_mday += interval;
}
