#ifndef SET_H
#define SET_H

#include <stddef.h>

/**
 * Implementation of an ordered LIST Abstract Data Type
 *
 * YOU HAVE TO GIVE TO THE SET (at the initialisation):
 *
 * A comparison function respecting the following rule:
 * int function( void* x,  void* y);
 * - x > y : 1
 * - x == y : 0
 * - x < y : -1
 *
 * A copy function for the type you want to use, using a malloc :
 * simple example (just query replace the <type>)
 * void* <type>_copy( void* x){
 *   <type> *y = x;
 *   <type>* y_copy = malloc(sizeof(type));
 *   *y_copy = *y;
 *   return y_copy;
 * }
 *
 * A free function for all the allocated memory used by the type
 * * void delete(void*);
 */

/**
 * @brief A sorted ADT to stock elements of the same type.
 * 
 */
struct set;


/**
 * @brief Set the empty object
 * 
 * @param copy_op is a copy function for the used type.
 * The copy function must have the following prototype and use a malloc:
 * void* copy_op(void*);
 * 
 * @param delete_op is a free function for all the allocated memory used by the type
 * The delete function must have the following prototype:
 * void delete_op(void*);
 * 
 * @param compare_op is a comparison function for the used type.
 * The comparison function must have the following prototype:
 * int cmp_op(void* x, void* y);
 * It have to respect the following rule:
 * - x > y : 1
 * - x == y : 0
 * - x < y : -1
 * 
 * @return An empty struct set* 
 */
struct set *set__empty(void* copy_op, void* delete_op, void* compare_op);


/**
 * @brief Test to know if a set is empty
 * 
 * @param s is the given set
 * @return 1 if the set is empty, 0 else
 */
int set__is_empty(const struct set *s);


/**
 * @brief Searches an object into the given set
 * 
 * @param s is the given set
 * @param e is the object you're searching
 * @return 1 if the object is in the set, 0 else
 */
int set__find(const struct set *s, void* e);


/**
 * @brief Give the current amount of objects in the set
 * 
 * @param s is the set
 * @return The amount of objects in the set
 */
size_t set__size(const struct set *s);


/**
 * @brief Adds an object into the given set
 * The object won't be added if it already is in the set.
 * 
 * @param s is the set
 * @param e is the object you want to add
 * @return 0 for a success, 1 for a failure
 */
int set__add(struct set *s, void* e);


/**
 * @brief Remove an object from the given set
 * 
 * @param s is the set
 * @param e is the object you want to remove
 * @return 0 for a success, 1 for a failure
 */
int set__remove(struct set *s, void* e);


/**
 * @brief Gets the pointer stocked into the set to the researched element
 * @note DO NOT FREE THAT POINTER !! Or the set__free will have a double-free issue.
 * 
 * @param s is the given set
 * @param e is the element you're looking for
 * @return A pointer to the data stocked by the set without a copy.
 */
void* set__retrieve(struct set *s, void* e);


/**
 * @brief Get the element in position i into the set.
 * 
 * @param set in the given set
 * @param i is the i th element into the set (0 <= i < set__size(set))
 * @return A copy of the i-th element stocked into the set if i is valid, NULL else.
 */
void* set__get_i_th(struct set const *set, size_t i);


/**
 * @brief Get the element in position i into the set.
 * @note DO NOT FREE THAT POINTER !! Or the set__free will have a double-free issue.
 *
 * @param set in the given set
 * @param i is the i th element into the set (0 <= i < set__size(set))
 * @return the i-th element stocked into the set if i is valid, NULL else.
 */
void* set__get_i_th_no_copy(struct set const *set, size_t i);


/**
 * @brief Free all the allocated memory in the set.
 * 
 * @param s is the set you want to free
 */
void set__free(struct set *s);


/**
 * @brief Creates a new filtered set according to a given a predicate
 * 
 * @param set is the set you want to filter from (it is not modified)
 * @param filter is the predicate for the filter.
 * It musts have the following prototype:
 * int filter (const void*);
 * 1 : Keep the object
 * 0 : Do not Keep the object
 * 
 * @return A new struct set* having only the filtered elements.
 */
struct set *set__filter(const struct set *set, int (*filter) (const void*));


/**
 * @brief Prints the set
 * 
 * @param set is the set you want to print
 * @param print_data is a function to print an object in the set
 * It musts have the following prototype:
 * void (*print_data) (const void*);
 * 
 */
void set__debug_data(const struct set *set, void (*print_data) (const void*));

#endif