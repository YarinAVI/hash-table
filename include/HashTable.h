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
/**
 * @brief HashTable type.
 * 
 */
typedef struct HashTable * HashTable;
/**
 * @brief HashTable Item.
 * 
 */
typedef void * HItem;
/**
 * @brief Constant HashTable Item.
 * 
 */
typedef const void * CHItem;


/**
 * @brief creates a new hash-table.
 * 
 * @param ctor a user defined constructor function for an HItem.
 * @param dtor a user defined destructor function for an HItem.
 * @param compar a user defined function to compare between two HItems,
 *               two HItems are identical to each other if this function returns 0.
 * @param HashFunct a user defined function to return a hash key for a speific HItem ( size_t).
 * @return HashTable the hash-table.
 */
HashTable newHashTable(HItem (*ctor)(CHItem),
             void (*dtor)(HItem),
             int (*compar)(CHItem,CHItem),
             size_t (*HashFunct)(CHItem));

/**
 * @brief finds an HItem in the hash-Table.
 * 
 * @param HT the hash-table.
 * @param clone a clone of the HItem to find.
 * @return HItem returns a reference to the HItem if found, NULL otherwise.
 */
HItem HashTableFind(HashTable HT,CHItem clone);

/**
 * @brief insert an HItem to the hash-table.
 * 
 * @param HT the hash-table.
 * @param clone a clone of the item to insert.
 * @return HItem returns a reference to the item in the hash-talbe if succeeded,NULL otherwise.
 */
HItem HashTableInsert(HashTable HT,CHItem clone);

/**
 * @brief deletes an HItem in the hash-table.
 * 
 * @param HT the hash-table.
 * @param clone a clone of the HItem to delete.
 */
void HashTableDelete(HashTable HT,CHItem clone);

/**
 * @brief deallocates the hash-table and its internals. 
 * 
 * @param HT the hash-table.
 */
void HashTableDestroy(HashTable HT);

/**
 * @brief 
 * 
 * @param HT 
 * @return HItem 
 */
HItem HashTableGetNextSet(HashTable HT);

/**
 * @brief 
 * 
 * @param HT 
 */
void HashTableResetIterator(HashTable HT);

/**
 * @brief 
 * 
 * @param HT 
 * @return size_t 
 */
size_t HashTableItemCount(HashTable HT);
#ifdef __cplusplus
}
#endif
#endif