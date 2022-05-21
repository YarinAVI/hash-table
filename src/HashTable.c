/*
 * @Author: Yarin Avisidris 
 * @Date: 2022-05-21 18:56:38 
 * @Last Modified by: Yarin Avisidris
 * @Last Modified time: 2022-05-21 20:00:59
 */
#include "../include/HashTable.h"
#include <stdlib.h>
#define BASE_ITEMS_FOR_EACH_BIN 2

/**
 * @brief Internal functionality of a bin, this is only used in here.
 * 
 */
typedef struct  {
    Item * Items;
    size_t SlotsNo; /* how many slots */
}Bin;

static Bin newBin(void) {
    Bin ret;
    ret.SlotsNo = BASE_ITEMS_FOR_EACH_BIN;
    ret.Items = calloc(ret.SlotsNo,sizeof(Item));
        if(!ret.Items) {
            ret.SlotsNo = 0;
            return ret;
        }
    return ret;
}
static int BinGrow(Bin *b) {
    Item * temp;
    size_t i = b->SlotsNo;
    b->SlotsNo *=2;
    temp = realloc(b->Items,b->SlotsNo * sizeof(Item));
    if(!temp) {
        b->SlotsNo /=2;
        return -1;
    }
    b->Items = temp;
    for(;i<b->SlotsNo;i++)
        b->Items[i] = NULL;
    return 0;
}
static Item BinInsert(Bin * b,CItem clone,Item(*ctor)(CItem)) {
    size_t i;
    /* look for an empty slot */
    for(i=0;i<b->SlotsNo && b->Items[i];i++);
    /* need to resize the bin */
    if(i == b->SlotsNo) {
        if(BinGrow(b) !=0) {
            return NULL;
        }
    }
    b->Items[i] = ctor(clone);
    return b->Items[i];
}
static Item * BinFind(Bin * b, CItem clone, int (*compar)(CItem,CItem)) {
    size_t i;
    for(i=0;i<b->SlotsNo;i++) {
        if(b->Items[i] && compar(clone,b->Items[i]) == 0) {
            return &(b->Items[i]);
        }
    }
    return NULL;
}
static void BinDelete(Bin *b,CItem clone, void (*dtor)(Item), int (*compar)(CItem,CItem)) {
    Item * target = BinFind(b,clone,compar);
    if(target) {
        dtor(*target);
        (*target) = NULL;
    }
}
static void BinDestroy(Bin *b, void (*dtor)(Item)) {
    size_t i;
    for(i=0;i<b->SlotsNo;i++) {
        if(b->Items[i])
            dtor(b->Items[i]);
    }
    b->SlotsNo = 0;
    free(b->Items);
}

/*===================================hash-table code===================================*/

/**
 * @brief HashTable is implemented using a list of bins,
 *        each bin is a list of Items, where the hash key maps to a specific "bin".
 */

#define BASE_NUMBER_OF_BINS     512

struct HashTable {
    Bin *   Bins;
    size_t  BinsNo; /* how many bins are being used in the hash table */

    Item (*ctor)(CItem);
    void (*dtor)(Item);
    int (*compar)(CItem,CItem);
    size_t (*HashFunct)(CItem);
};

HashTable newHashTable(Item (*ctor)(CItem),
             void (*dtor)(Item),
             int (*compar)(CItem,CItem),
             size_t (*HashFunct)(CItem)) {
    size_t i,j;
    /* allocate memory for the ht struct */
    HashTable HTret = calloc(1,sizeof(struct HashTable));
    if(!HTret)
        return NULL;
    HTret->BinsNo = BASE_NUMBER_OF_BINS;
    HTret->Bins = calloc(HTret->BinsNo,sizeof(Bin)); 
        if(!HTret->Bins) {
            free(HTret);
            return NULL;
        }
    /* allocate space for each bin */
    for(i=0;i<HTret->BinsNo;i++) {
        HTret->Bins[i] = newBin();
        /* if one of the bins allocation failed, dealloc every bin and return NULL. */
        if(HTret->Bins[i].SlotsNo == 0) {
            for(j=0;j<i;j++) {
                BinDestroy(&HTret->Bins[i],dtor);
            }
            free(HTret->Bins);
            free(HTret);
            return NULL;
        }
    }
    HTret->compar       = compar;
    HTret->ctor         = ctor;
    HTret->dtor         = dtor;
    HTret->HashFunct    = HashFunct;
    return HTret;
}

void HashTableDestroy(HashTable HT) {
    size_t i;
    for(i =0;i<HT->BinsNo;i++) {
        if(HT->Bins[i].SlotsNo)
            BinDestroy(&HT->Bins[i],HT->dtor);
    }
    free(HT->Bins);
    free(HT);
}

Item HashTableFind(HashTable HT,CItem clone) {
    /* first get the hash key for the specific clone*/
    size_t TargetBin = HT->HashFunct(clone) % HT->BinsNo;
    Item * TargetItem = BinFind(&HT->Bins[TargetBin],clone,HT->compar);
    if(TargetItem) {
        return *TargetItem;
    }
    return NULL;
}
Item HashTableInsert(HashTable HT,CItem clone) {
    /* first get the hash key for the specific clone*/
    size_t TargetBin = HT->HashFunct(clone) % HT->BinsNo;
    return BinInsert(&HT->Bins[TargetBin],clone,HT->ctor);
}
void HashTableDelete(HashTable HT,CItem clone) {
    /* first get the hash key for the specific clone*/
    size_t TargetBin = HT->HashFunct(clone) % HT->BinsNo;
    BinDelete(&HT->Bins[TargetBin],clone,HT->dtor,HT->compar);
}