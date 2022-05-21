/*
 * @Author: Yarin Avisidris 
 * @Date: 2022-05-21 18:56:47 
 * @Last Modified by: Yarin Avisidris
 * @Last Modified time: 2022-05-21 20:03:41
 */

#ifndef __HASH_TABLE__H_
#define __HASH_TABLE__H_
#include <stddef.h>
#ifdef __cplusplus
extern "C"
{
#endif
typedef void * Item;
typedef const void * CItem;
typedef struct HashTable * HashTable;

/**
 * @brief creates a new hash-table.
 * 
 * @param ctor a user defined constructor function for an Item.
 * @param dtor a user defined destructor function for an Item.
 * @param compar a user defined function to compare between two Items,
 *               two Items are identical to each other if this function returns 0.
 * @param HashFunct a user defined function to return a hash key for a speific Item ( size_t).
 * @return HashTable the hash-table.
 */
HashTable newHashTable(Item (*ctor)(CItem),
             void (*dtor)(Item),
             int (*compar)(CItem,CItem),
             size_t (*HashFunct)(CItem));

/**
 * @brief finds an Item in the hash-Table.
 * 
 * @param HT the hash-table.
 * @param clone a clone of the Item to find.
 * @return Item returns a reference to the Item if found, NULL otherwise.
 */
Item HashTableFind(HashTable HT,CItem clone);

/**
 * @brief insert an Item to the hash-table.
 * 
 * @param HT the hash-table.
 * @param clone a clone of the item to insert.
 * @return Item returns a reference to the item in the hash-talbe if succeeded,NULL otherwise.
 */
Item HashTableInsert(HashTable HT,CItem clone);

/**
 * @brief deletes an Item in the hash-table.
 * 
 * @param HT the hash-table.
 * @param clone a clone of the Item to delete.
 */
void HashTableDelete(HashTable HT,CItem clone);

/**
 * @brief deallocates the hash-table and its internals. 
 * 
 * @param HT the hash-table.
 */
void HashTableDestroy(HashTable HT);

#ifdef __cplusplus
}
#endif
#endif