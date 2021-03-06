#include "common_tests_utils.h"
#include "../src/common/card.h"
#include "../src/common/ADT/stack.h"
#include "../src/common/utils.h"

int test_card__init()
{
    printf("%s... ", __func__);

    struct card* c = card__init(CARD_JUNCTION_CITY);
    if (c->type.id != CARD_JUNCTION_CITY) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    if (c->direction != DEFAULT_DIRECTION) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    card__free(c);

    return TEST_SUCCESS;
}

int test_card__get_area()
{
    printf("%s... ", __func__);

    struct card *c = card__init(FIRST_CARD_ID);
    c->direction = WEST;

    if (card__get_relative_area(c, POS_NORTH) != CITY) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    if (card__get_relative_area(c, POS_EAST) != ROAD) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    if (card__get_relative_area(c, POS_CENTER) != ROAD) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    if (card__get_relative_area(c, (enum place) 50) != INVALID_AREA) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    card__free(c);

    return TEST_SUCCESS;

}

int test_card__get_neighbour_number()
{
    printf("%s... ", __func__);

    struct card *c = card__init(CARD_MONASTERY_ALONE);

    if (card__get_neighbour_number(c) != 0) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    c->neighbors[0] = c; //stupid but enough for the test

    if (card__get_neighbour_number(c) != 1) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    card__free(c);

    return TEST_SUCCESS;
}

int test_card__get_position_at_side()
{
    printf("%s... ", __func__);

    struct card *c = card__init(CARD_MONASTERY_ALONE);
    c->pos = (struct position){ 5, -9 };

    if (card__get_position_at_direction(c, NORTH).x != 5
            || card__get_position_at_direction(c, NORTH).y != -8) {
        card__free(c);
        return !TEST_SUCCESS;
    }
    if (card__get_position_at_direction(c, SOUTH).x != 5
            || card__get_position_at_direction(c, SOUTH).y != -10) {
        card__free(c);
        return !TEST_SUCCESS;
    }
    if (card__get_position_at_direction(c, WEST).x != 4
            || card__get_position_at_direction(c, WEST).y != -9) {
        card__free(c);
        return !TEST_SUCCESS;
    }
    if (card__get_position_at_direction(c, EAST).x != 6
            || card__get_position_at_direction(c, EAST).y != -9) {
        card__free(c);
        return !TEST_SUCCESS;
    }

    card__free(c);
    return TEST_SUCCESS;
}

int test_card__are_matching_sides_success()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_CITY_THREE_SHLD);
    struct card *c2 = card__init(CARD_CITY_ALL_SIDES);

    if (!card__are_matching_directions(c1, c2, WEST, EAST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return TEST_SUCCESS;
}

int test_card__are_matching_sides_failure()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ALONE);
    struct card *c2 = card__init(CARD_JUNCTION_THREE);

    if (card__are_matching_directions(c1, c1, EAST, WEST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__are_matching_directions(c1, c2, EAST, SOUTH)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__are_matching_directions(c1, c2, EAST, (enum direction) 99)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return TEST_SUCCESS;
}

int test_card__link_at_side()
{
    printf("%s... ", __func__);

    struct card *c1 = card__init(CARD_MONASTERY_ROAD);
    struct card *c2 = card__init(CARD_JUNCTION_THREE);

    if (card__link_at_directions(c1, c2, SOUTH, EAST)
            && (c1->neighbors[SOUTH] != c2
            ||c2->neighbors[EAST] != c1)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_directions(c1, c2, SOUTH, EAST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_directions(c1, c1, EAST, WEST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    if (card__link_at_directions(c1, c2, EAST + 1, EAST)) {
        card__free(c1);
        card__free(c2);
        return !TEST_SUCCESS;
    }

    card__free(c1);
    card__free(c2);

    return TEST_SUCCESS;
}

int test_card__unlink_neighbours()
{
    printf("%s... ", __func__);

    int result = TEST_SUCCESS;

    struct card *c_up = card__init(CARD_MONASTERY_ALONE);
    struct card *c_center = card__init(CARD_JUNCTION_FOUR);
    struct card *c_right = card__init(CARD_ROAD_TURN);
    struct card *c_down = card__init(CARD_ROAD_STRAIGHT);
    struct card *c_left = card__init(CARD_ROAD_STRAIGHT);
    c_left->direction = WEST;

    if (!card__link_at_directions(c_center, c_up, NORTH, SOUTH)) result = !TEST_SUCCESS;
    if (!card__link_at_directions(c_center, c_right, EAST, WEST)) result = !TEST_SUCCESS;
    if (!card__link_at_directions(c_center, c_down, SOUTH, NORTH)) result = !TEST_SUCCESS;
    if (!card__link_at_directions(c_center, c_left, WEST, EAST)) result = !TEST_SUCCESS;

    card__unlink_neighbours(c_center);

    if (c_center->neighbors[NORTH] != NULL || c_up->neighbors[SOUTH] != NULL) result = !TEST_SUCCESS;
    if (c_center->neighbors[EAST] != NULL || c_right->neighbors[WEST] != NULL) result = !TEST_SUCCESS;
    if (c_center->neighbors[SOUTH] != NULL || c_down->neighbors[NORTH] != NULL) result = !TEST_SUCCESS;
    if (c_center->neighbors[WEST] != NULL || c_left->neighbors[EAST] != NULL) result = !TEST_SUCCESS;

    card__free(c_center);
    card__free(c_up);
    card__free(c_right);
    card__free(c_down);
    card__free(c_left);

    return result;
}


int main()
{
    printf("----------- TEST CARD -----------\n");

    int nb_success = 0;
    int nb_tests = 0;

    print_test_result(test_card__init(), &nb_success, &nb_tests);
    print_test_result(test_card__get_area(), &nb_success, &nb_tests);
    print_test_result(test_card__get_neighbour_number(), &nb_success, &nb_tests);
    print_test_result(test_card__get_position_at_side(), &nb_success, &nb_tests);
    print_test_result(test_card__are_matching_sides_success(), &nb_success, &nb_tests);
    print_test_result(test_card__are_matching_sides_failure(), &nb_success, &nb_tests);
    print_test_result(test_card__link_at_side(), &nb_success, &nb_tests);
    print_test_result(test_card__unlink_neighbours(), &nb_success, &nb_tests);

    print_test_summary(nb_success, nb_tests);

    return TEST_SUCCESS;
}
