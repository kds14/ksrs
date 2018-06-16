#ifndef REPS_H
#define REPS_H

#include "cards.h"

int queue_count;

enum answer { RIGHT, WRONG };

struct card *next_rep();
void add_rep(struct card *card);
void answer_card(enum answer ans, struct card *card);

#endif
