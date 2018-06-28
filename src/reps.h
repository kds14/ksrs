#ifndef REPS_H
#define REPS_H

#include "cards.h"

int queue_count;

enum answer { RIGHT, WRONG };

void add_rep_if_due(struct card *card);
struct card	*next_rep();
void answer_card(enum answer ans, struct card *card);

#endif
