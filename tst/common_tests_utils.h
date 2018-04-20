#ifndef COMMON_TESTS_UTILS_H
#define COMMON_TESTS_UTILS_H

#ifndef SUCCESS
#define SUCCESS 0
#endif

#define TEST_SUCCESS 0

#include "../src/common/common_interface.h"
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////
///     COMMON TESTS FUNCTIONS
////////////////////////////////////////////////////////////////////

/**
 * @brief Print if a test succeeded or not, in color
 * @param result the test result
 */
void print_success(int result);

////////////////////////////////////////////////////////////////////
///     OPERATORS FOR ADT TESTS
////////////////////////////////////////////////////////////////////

enum card_id;

void* operator_copy(const enum card_id *p_card_id);
void operator_delete(enum card_id *p_card_id);
int operator_compare(const enum card_id *ci1, const enum card_id *ci2);
void operator_debug(const enum card_id *p_card_id);


#endif