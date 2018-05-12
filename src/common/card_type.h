#ifndef CARD_TYPE_DEF_H
#define CARD_TYPE_DEF_H

#include "common_interface.h"
#include "area_type.h"

#define MAX_ZONES 13
#define MAX_SIDE_ZONES 12
#define FIRST_CARD_ID CARD_ROAD_STRAIGHT_CITY

////////////////////////////////////////////////////////////////////
///     STRUCTURES
////////////////////////////////////////////////////////////////////

/**
 * @brief card_type
 * Describes the card of that type
 */
struct card_type
{
    enum card_id id;
    enum area_type areas[MAX_ZONES];
};


////////////////////////////////////////////////////////////////////
///     CARD TYPE FUNCTIONS
////////////////////////////////////////////////////////////////////

struct card_type card__id_to_type(enum card_id id);


#endif
