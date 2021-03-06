#include "common_tests_utils.h"
#include "../src/server/board.h"
#include "../src/common/ADT/set.h"
#include "../src/common/deck.h"

int test_board__empty()
{
    printf("%s... ", __func__);

    struct board* b = board__init();

    if (!set__is_empty(b->cards_set) || !set__is_empty(b->meeples_set)
            || !queue__is_empty(b->moves_queue) || !stack__is_empty(b->drawing_stack)) {
        board__free(b);
        return !TEST_SUCCESS;
    }

    board__free(b);

    return TEST_SUCCESS;
}

int test_board__init_first_card()
{
    printf("%s... ", __func__);

    struct board* b = board__init();

    if (b->first_card != NULL || !set__is_empty(b->cards_set)) {
        board__free(b);
        return !TEST_SUCCESS;
    }

    if (board__init_deck_and_first_card(NULL) == SUCCESS) {
        board__free(b);
        return !TEST_SUCCESS;
    }

    board__init_deck_and_first_card(b);


    if (b->first_card == NULL || set__is_empty(b->cards_set)) {
        board__free(b);
        return !TEST_SUCCESS;
    }

    board__free(b);

    return TEST_SUCCESS;
}

int test_board__is_valid_card()
{
    printf("%s... ", __func__);

    if (board__is_valid_card(NULL, CARD_ROAD_TURN_RIGHT_CITY))
        return !TEST_SUCCESS;

    struct board* b = board__init();
    if (board__is_valid_card(b, LAST_CARD)) {
        board__free(b);
        return !TEST_SUCCESS;
    }

    struct card* c1 = card__init(CARD_ROAD_TURN);

    set__add(b->cards_set, c1);

    if (!board__is_valid_card(b, CARD_MONASTERY_ALONE)) {
        board__free(b);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    if (board__is_valid_card(b, CARD_CITY_ALL_SIDES)) {
        board__free(b);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);

    return TEST_SUCCESS;
}

int test_board__retrieve_card_by_position()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_MONASTERY_ALONE, (struct position){ 0, 0}, NORTH);
    struct card *c1 = card__init(CARD_JUNCTION_THREE);
    c1->pos.x = 0;
    c1->pos.y = -1;
    board__add_card(b, c1);

    struct card *retrieved = board__retrieve_card_by_position(b->cards_set, c1->pos);

    if (retrieved == NULL || retrieved->type.id != CARD_JUNCTION_THREE) {
        card__free(c1);
        board__free(b);

        return !TEST_SUCCESS;
    }

    card__free(c1);
    board__free(b);

    return TEST_SUCCESS;
}

int test_board__add_card__empty_set()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    struct card *c1 = card__init(CARD_MONASTERY_ROAD);
    c1->pos.x = 0;
    c1->pos.y = 1;

    if (board__add_card(NULL, c1) == SUCCESS) {
        board__free(b);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    if (board__add_card(b, NULL) == SUCCESS) {
        board__free(b);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    if (board__add_card(b, c1) == SUCCESS || card__get_neighbour_number(c1) != 0) {
        board__free(b);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);

    return TEST_SUCCESS;
}

int test_board__add_card__non_empty_set_no_match()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_CITY_ALL_SIDES, (struct position){ 0, 0}, NORTH);
    struct card *c1 = card__init(CARD_MONASTERY_ALONE);
    c1->pos.x = 0;
    c1->pos.y = 1;

    if (board__add_card(b, c1) == SUCCESS || card__get_neighbour_number(c1) != 0) {
        board__free(b);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);

    return TEST_SUCCESS;
}

int test_board__add_card__non_empty_set_with_match()
{
    printf("%s... ", __func__);

    struct board *b = board__init();

    board__add_custom_first_card(b, CARD_ROAD_STRAIGHT_CITY, (struct position){ 0, 0}, WEST);

    struct card *c1 = card__init(CARD_CITY_TUNNEL);
    c1->pos.x = 0;
    c1->pos.y = 1;

    int adding_result = board__add_card(b, c1);
    struct card *saved_c1 = set__retrieve(b->cards_set, c1);   //Positioned Above starting card
    if (adding_result != SUCCESS
            || card__get_neighbour_number(saved_c1) != 1
            || saved_c1->neighbors[SOUTH] != b->first_card) {
        board__free(b);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);

    return TEST_SUCCESS;
}

int test_board__add_card__non_empty_set_with_match_twice()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_ROAD_STRAIGHT_CITY, (struct position){ 0, 0}, WEST);

    struct card *c1 = card__init(CARD_CITY_TUNNEL);
    c1->pos.x = 0;
    c1->pos.y = 1;
    board__add_card(b, c1);

    //test adding same twice

    if (board__add_card(b, c1) == SUCCESS) {
        board__free(b);
        card__free(c1);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);

    return TEST_SUCCESS;
}

int test_board__add_card__non_empty_set()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_ROAD_STRAIGHT_CITY, (struct position){ 0, 0}, WEST);

    //=== Adding until having a full surrounding for CARD_CITY_TUNNEL

    int test_result = !TEST_SUCCESS;
    struct card *c1 = NULL, *c2 = NULL, *c3 = NULL, *c4 = NULL, *c5 = NULL, *c6 = NULL, *c7 = NULL, *c8 = NULL;

    c1 = card__init(CARD_CITY_TUNNEL);
    c1->pos.x = 0;
    c1->pos.y = 1;
    c1->direction = NORTH;
    if (board__add_card(b, c1) == SUCCESS) {
        c2 = card__init(CARD_CITY_THREE);
        c2->pos.x = 0;
        c2->pos.y = 2;
        c2->direction = SOUTH;
        if (board__add_card(b, c2) == SUCCESS) {
            c3 = card__init(CARD_ROAD_TURN_RIGHT_CITY);
            c3->pos.x = -1;
            c3->pos.y = 2;
            c3->direction = EAST;
            if (board__add_card(b, c3) == SUCCESS) {
                c4 = card__init(CARD_ROAD_STRAIGHT);
                c4->pos.x = -1;
                c4->pos.y = 1;
                c4->direction = NORTH;
                if (board__add_card(b, c4) == SUCCESS) {
                    c5 = card__init(CARD_JUNCTION_FOUR);
                    c5->pos.x = -1;
                    c5->pos.y = 0;
                    c5->direction = NORTH;
                    if (board__add_card(b, c5) == SUCCESS) {
                        c6 = card__init(CARD_PLAIN_CITY_ROAD);
                        c6->pos.x = 1;
                        c6->pos.y = 2;
                        c6->direction = NORTH;
                        if (board__add_card(b, c6) == SUCCESS) {
                            c7 = card__init(CARD_MONASTERY_ROAD);
                            c7->pos.x = 1;
                            c7->pos.y = 1;
                            c7->direction = SOUTH;
                            if (board__add_card(b, c7) == SUCCESS) {
                                c8 = card__init(CARD_JUNCTION_THREE);
                                c8->pos.x = 1;
                                c8->pos.y = 0;
                                c8->direction = NORTH;
                                if (board__add_card(b, c8) == SUCCESS)
                                    test_result = TEST_SUCCESS;
                            }
                        }
                    }
                }
            }
        }
    }

    if (test_result == TEST_SUCCESS) {

        //=== Card link verification

        //With previous test, we have made a 3*3 grid of cards, coordinates are indicated below
        //We recover below the current cards because the set in board stores copies of the cards we have added :

        struct card * cur_c0 = b->first_card;                   //SC
        struct card * cur_c1 = set__retrieve(b->cards_set, c1); //C
        struct card * cur_c2 = set__retrieve(b->cards_set, c2); //NC
        struct card * cur_c3 = set__retrieve(b->cards_set, c3); //NW
        struct card * cur_c4 = set__retrieve(b->cards_set, c4); //CW
        struct card * cur_c5 = set__retrieve(b->cards_set, c5); //SW
        struct card * cur_c6 = set__retrieve(b->cards_set, c6); //NE
        struct card * cur_c7 = set__retrieve(b->cards_set, c7); //CE
        struct card * cur_c8 = set__retrieve(b->cards_set, c8); //SE

        //Neighbours count test :

        if (card__get_neighbour_number(cur_c0) != 3) test_result = !TEST_SUCCESS;
        if (card__get_neighbour_number(cur_c1) != 4) test_result = !TEST_SUCCESS;
        if (card__get_neighbour_number(cur_c2) != 3) test_result = !TEST_SUCCESS;
        if (card__get_neighbour_number(cur_c3) != 2) test_result = !TEST_SUCCESS;
        if (card__get_neighbour_number(cur_c4) != 3) test_result = !TEST_SUCCESS;
        if (card__get_neighbour_number(cur_c5) != 2) test_result = !TEST_SUCCESS;
        if (card__get_neighbour_number(cur_c6) != 2) test_result = !TEST_SUCCESS;
        if (card__get_neighbour_number(cur_c7) != 3) test_result = !TEST_SUCCESS;
        if (card__get_neighbour_number(cur_c8) != 2) test_result = !TEST_SUCCESS;

        //Linking test :

        if (cur_c0->neighbors[NORTH] != cur_c1 || cur_c0->neighbors[WEST] != cur_c5
                || cur_c0->neighbors[SOUTH] != NULL || cur_c0->neighbors[EAST] != cur_c8)
            test_result = !TEST_SUCCESS;

        if (cur_c1->neighbors[NORTH] != cur_c2 || cur_c1->neighbors[WEST] != cur_c4
                || cur_c1->neighbors[SOUTH] != cur_c0 || cur_c1->neighbors[EAST] != cur_c7)
            test_result = !TEST_SUCCESS;

        if (cur_c2->neighbors[NORTH] != NULL || cur_c2->neighbors[WEST] != cur_c3
                || cur_c2->neighbors[SOUTH] != cur_c1 || cur_c2->neighbors[EAST] != cur_c6)
            test_result = !TEST_SUCCESS;

        if (cur_c3->neighbors[NORTH] != NULL || cur_c3->neighbors[WEST] != NULL
                || cur_c3->neighbors[SOUTH] != cur_c4 || cur_c3->neighbors[EAST] != cur_c2)
            test_result = !TEST_SUCCESS;

        if (cur_c4->neighbors[NORTH] != cur_c3 || cur_c4->neighbors[WEST] != NULL
                || cur_c4->neighbors[SOUTH] != cur_c5 || cur_c4->neighbors[EAST] != cur_c1)
            test_result = !TEST_SUCCESS;

        if (cur_c5->neighbors[NORTH] != cur_c4 || cur_c5->neighbors[WEST] != NULL
                || cur_c5->neighbors[SOUTH] != NULL || cur_c5->neighbors[EAST] != cur_c0)
            test_result = !TEST_SUCCESS;

        if (cur_c6->neighbors[NORTH] != NULL || cur_c6->neighbors[WEST] != cur_c2
                || cur_c6->neighbors[SOUTH] != cur_c7 || cur_c6->neighbors[EAST] != NULL)
            test_result = !TEST_SUCCESS;

        if (cur_c7->neighbors[NORTH] != cur_c6 || cur_c7->neighbors[WEST] != cur_c1
                || cur_c7->neighbors[SOUTH] != cur_c8 || cur_c7->neighbors[EAST] != NULL)
            test_result = !TEST_SUCCESS;

        if (cur_c8->neighbors[NORTH] != cur_c7 || cur_c8->neighbors[WEST] != cur_c0
                || cur_c8->neighbors[SOUTH] != NULL || cur_c8->neighbors[EAST] != NULL)
            test_result = !TEST_SUCCESS;
    }


    //== Freeing resources

    card__free(c1);
    card__free(c2);
    card__free(c3);
    card__free(c4);
    card__free(c5);
    card__free(c6);
    card__free(c7);
    card__free(c8);
    board__free(b);

    return test_result;
}

int test_board__add_card__middle()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_CITY_ALL_SIDES, (struct position){ 5, 5}, WEST);

    int test_result = TEST_SUCCESS;
    struct card *c2 = NULL, *c3 = NULL, *c4 = NULL, *c5 = NULL, *c6 = NULL, *c7 = NULL, *c8 = NULL, *c9 = NULL;

    c2 = card__init(CARD_CITY_TUNNEL);
    c2->pos.x = 6;
    c2->pos.y = 5;
    c2->direction = EAST;
    if (board__add_card(b, c2) == SUCCESS) {
        c3 = card__init(CARD_ROAD_TURN_LEFT_CITY);
        c3->pos.x = 7;
        c3->pos.y = 5;
        c3->direction = SOUTH;
        if (board__add_card(b, c3) == SUCCESS) {
            c4 = card__init(CARD_JUNCTION_FOUR);
            c4->pos.x = 7;
            c4->pos.y = 4;
            c4->direction = NORTH;
            if (board__add_card(b, c4) == SUCCESS) {
                c5 = card__init(CARD_MONASTERY_ROAD);
                c5->pos.x = 7;
                c5->pos.y = 3;
                c5->direction = SOUTH;
                if (board__add_card(b, c5) == SUCCESS) {
                    c6 = card__init(CARD_ROAD_STRAIGHT);
                    c6->pos.x = 6;
                    c6->pos.y = 3;
                    c6->direction = NORTH;
                    if (board__add_card(b, c6) == SUCCESS) {
                        c7 = card__init(CARD_PLAIN_CITY);
                        c7->pos.x = 5;
                        c7->pos.y = 3;
                        c7->direction = NORTH;
                        if (board__add_card(b, c7) == SUCCESS) {
                            c8 = card__init(CARD_CITY_THREE_ROAD);
                            c8->pos.x = 5;
                            c8->pos.y = 4;
                            c8->direction = WEST;
                            if (board__add_card(b, c8) == SUCCESS) {
                                c9 = card__init(CARD_MONASTERY_ROAD);
                                c9->pos.x = 6;
                                c9->pos.y = 4;
                                c9->direction = NORTH;
                                if (board__add_card(b, c9) == SUCCESS)
                                    test_result = !TEST_SUCCESS;
                                c9->type = card_types[CARD_JUNCTION_THREE];
                                if (board__add_card(b, c9) != SUCCESS)
                                    test_result = !TEST_SUCCESS;
                            }
                        }
                    }
                }
            }
        }
    }

    card__free(c2);
    card__free(c3);
    card__free(c4);
    card__free(c5);
    card__free(c6);
    card__free(c7);
    card__free(c8);
    card__free(c9);
    board__free(b);

    return test_result;
}

int test_board__add_meeple_success()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_ROAD_STRAIGHT_CITY, (struct position){ 0, 0}, WEST);

    struct card *c1 = card__init(CARD_CITY_TUNNEL);
    c1->pos.x = 0;
    c1->pos.y = 1;
    board__add_card(b, c1);

    struct meeple *m = meeple__init(0, c1, POS_CENTER);


    if (board__add_meeple(b, m) != SUCCESS) {
        board__free(b);
        card__free(c1);
        meeple__free(m);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);
    meeple__free(m);

    return TEST_SUCCESS;
}

int test_board__add_meeple_unlinked_card_failure()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_ROAD_STRAIGHT_CITY, (struct position){ 0, 0}, WEST);

    struct card *c1 = card__init(CARD_CITY_TUNNEL);
    c1->pos.x = 0;
    c1->pos.y = 1;
    //We don't add the card to the set

    struct meeple *m = meeple__init(0, c1, POS_CENTER);


    if (board__add_meeple(b, m) == SUCCESS) {
        board__free(b);
        card__free(c1);
        meeple__free(m);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);
    meeple__free(m);

    return TEST_SUCCESS;
}

int test_board__add_meeple_twice_on_card_failure()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_ROAD_STRAIGHT_CITY, (struct position){ 0, 0}, WEST);

    struct card *c1 = card__init(CARD_CITY_TUNNEL);
    c1->pos.x = 0;
    c1->pos.y = 1;
    board__add_card(b, c1);

    struct meeple *m = meeple__init(0, c1, POS_CENTER);

    board__add_meeple(b, m); //first adding which succeed
    if (board__add_meeple(b, m) == SUCCESS) { //second adding which shall fail
        board__free(b);
        card__free(c1);
        meeple__free(m);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);
    meeple__free(m);

    return TEST_SUCCESS;
}

int test_board__add_meeple_twice_on_zone_failure()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_ROAD_STRAIGHT_CITY, (struct position){ 0, 0}, WEST);

    struct card *c1 = card__init(CARD_CITY_TUNNEL);
    c1->pos.x = 0;
    c1->pos.y = 1;
    board__add_card(b, c1);

    struct card *c2 = card__init(CARD_CITY_ALL_SIDES);
    c1->pos.x = 0;
    c1->pos.y = 2;
    board__add_card(b, c2);

    struct meeple *m1 = meeple__init(0, c1, POS_CENTER);
    struct meeple *m2 = meeple__init(1, c2, POS_WEST_NORTH);

    board__add_meeple(b, m1); //first adding which succeed
    if (board__add_meeple(b, m2) == SUCCESS) { //adding on another card of same zone which shall fail
        board__free(b);
        card__free(c1);
        card__free(c2);
        meeple__free(m1);
        meeple__free(m2);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);
    card__free(c2);
    meeple__free(m1);
    meeple__free(m2);

    return TEST_SUCCESS;
}

int test_board__add_meeple_on_other_zone_success()
{
    printf("%s... ", __func__);

    struct board *b = board__init();
    board__add_custom_first_card(b, CARD_ROAD_STRAIGHT_CITY, (struct position){ 0, 0}, WEST);

    struct card *c1 = card__init(CARD_CITY_TUNNEL);
    c1->pos.x = 0;
    c1->pos.y = 1;
    board__add_card(b, c1);

    struct card *c2 = card__init(CARD_CITY_ALL_SIDES);
    c1->pos.x = 0;
    c1->pos.y = 2;
    board__add_card(b, c2);

    struct meeple *m1 = meeple__init(0, c1, POS_WEST);
    struct meeple *m2 = meeple__init(1, c2, POS_WEST_NORTH);

    board__add_meeple(b, m1); //Adding on a plain which succeed
    if (board__add_meeple(b, m2) != SUCCESS) { //adding on another card on another zone (CITY) which shall succeed
        board__free(b);
        card__free(c1);
        card__free(c2);
        meeple__free(m1);
        meeple__free(m2);
        return !TEST_SUCCESS;
    }

    board__free(b);
    card__free(c1);
    card__free(c2);
    meeple__free(m1);
    meeple__free(m2);

    return TEST_SUCCESS;
}

int main()
{
    printf("----------- TEST BOARD -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_board__empty(), &nb_success, &nb_tests);
    print_test_result(test_board__init_first_card(), &nb_success, &nb_tests);
    print_test_result(test_board__is_valid_card(), &nb_success, &nb_tests);
    print_test_result(test_board__retrieve_card_by_position(), &nb_success, &nb_tests);
    print_test_result(test_board__add_card__empty_set(), &nb_success, &nb_tests);
    print_test_result(test_board__add_card__non_empty_set_no_match(), &nb_success, &nb_tests);
    print_test_result(test_board__add_card__non_empty_set_with_match(), &nb_success, &nb_tests);
    print_test_result(test_board__add_card__non_empty_set_with_match_twice(), &nb_success, &nb_tests);
    print_test_result(test_board__add_card__non_empty_set(), &nb_success, &nb_tests);
    print_test_result(test_board__add_card__middle(), &nb_success, &nb_tests);
    print_test_result(test_board__add_meeple_success(), &nb_success, &nb_tests);
    print_test_result(test_board__add_meeple_unlinked_card_failure(), &nb_success, &nb_tests);
    print_test_result(test_board__add_meeple_twice_on_card_failure(), &nb_success, &nb_tests);
    print_test_result(test_board__add_meeple_twice_on_zone_failure(), &nb_success, &nb_tests);
    print_test_result(test_board__add_meeple_on_other_zone_success(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
