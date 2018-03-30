#include "../common/projCarcassonneInterface.h"
#include <stdio.h>

/////////////////////////////////////////////////////////
/// Implementation of interface functions
/////////////////////////////////////////////////////////

char const* get_player_name()
{
    return("Team 4410 - Joueur 2");
}

void initialize(unsigned int id, unsigned int n_players)
{
    (void) id;
    (void) n_players;
    printf("Placeholder execution of \"initialize\" for %s\n", get_player_name());
}

struct move play(enum card_id card, struct move const previous_moves[], size_t n_moves)
{
    (void) card;
    (void) previous_moves;
    (void) n_moves;
    printf("Placeholder execution of \"play\" for %s\n", get_player_name());

    struct move move_placeholder = { FAILED, 0, CARD_PLAIN_CITY, { 0, 0 }, SOUTH, POS_WEST_NORTH};
    return move_placeholder;
}


void finalize()
{
    printf("Placeholder execution of \"finalize\" for %s\n", get_player_name());
}