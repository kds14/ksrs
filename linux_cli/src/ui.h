#ifndef UI_H
#define UI_H

#include "cards.h"

enum input_state { MAIN, REPS, ADD };
enum card_state { FRONT, BACK };
enum get_state { SING, REG };
struct app_state {
	enum input_state in_s;
	enum card_state card_s;
	enum card_state add_s;
	enum get_state getc_s;
};

int loaded_deck(int df);
void handle_main(char c, struct app_state *aps);
void handle_reps(char c, struct app_state *aps);
int handle_add(char c, struct app_state *aps);
#endif
