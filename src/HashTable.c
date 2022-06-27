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
    HItem * HItems;
    size_t SlotsNo; /* how many slots */
}Bin;

static Bin newBin(void) {
    Bin ret;
    ret.SlotsNo = BASE_ITEMS_FOR_EACH_BIN;
    ret.HItems = calloc(ret.SlotsNo,sizeof(HItem));
        if(!ret.HItems) {
            ret.SlotsNo = 0;
            return ret;
        }
    return ret;
}
static int BinGrow(Bin *b) {
    HItem * temp;
    size_t i = b->SlotsNo;
    b->SlotsNo *=2;
    temp = realloc(b->HItems,b->SlotsNo * sizeof(HItem));
    if(!temp) {
        b->SlotsNo /=2;
        return -1;
    }
    b->HItems = temp;
    for(;i<b->SlotsNo;i++)
        b->HItems[i] = NULL;
    return 0;
}
static HItem BinInsert(Bin * b,CHItem clone,HItem(*ctor)(CHItem)) {
    size_t i;
    /* look for an empty slot */
    for(i=0;i<b->SlotsNo && b->HItems[i];i++);
    /* need to resize the bin */
    if(i == b->SlotsNo) {
        if(BinGrow(b) !=0) {
            return NULL;
        }
    }
    b->HItems[i] = ctor(clone);
    return b->HItems[i];
}
static HItem * BinFind(Bin * b, CHItem clone, int (*compar)(CHItem,CHItem)) {
    size_t i;
    for(i=0;i<b->SlotsNo;i++) {
        if(b->HItems[i] && compar(clone,b->HItems[i]) == 0) {
            return &(b->HItems[i]);
        }
    }
    return NULL;
}
static void BinDelete(Bin *b,CHItem clone, void (*dtor)(HItem), int (*compar)(CHItem,CHItem)) {
    HItem * target = BinFind(b,clone,compar);
    if(target) {
        if(dtor)
            dtor(*target);
        (*target) = NULL;
    }
}
static void BinDestroy(Bin *b, void (*dtor)(HItem)) {
    size_t i;
    if(dtor) {
        for(i=0;i<b->SlotsNo;i++) {
            if(b->HItems[i])
                dtor(b->HItems[i]);
        }
    }
    b->SlotsNo = 0;
    free(b->HItems);
}

/*===================================hash-table code===================================*/

/**
 * @brief HashTable is implemented using a list of bins,
 *        each bin is a list of HItems, where the hash key maps to a specific "bin".
 */

#define BASE_NUMBER_OF_BINS     128

struct HashTable {
    Bin *   Bins;
    size_t  BinsNo; /* how many bins are being used in the hash table */

    HItem * Iterator;
    Bin *   BinIterator;

    size_t  ItemCount;
    HItem   (*ctor)(CHItem);
    void    (*dtor)(HItem);
    int     (*compar)(CHItem,CHItem);
    size_t  (*HashFunct)(CHItem);
};

HashTable newHashTable(HItem (*ctor)(CHItem),
             void (*dtor)(HItem),
             int (*compar)(CHItem,CHItem),
             size_t (*HashFunct)(CHItem)) {
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
                BinDestroy(&HTret->Bins[j],dtor);
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
    HTret->BinIterator = HTret->Bins;
    HTret->Iterator    = HTret->BinIterator->HItems;
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
HItem HashTableGetNextSet(HashTable HT) {
    HItem ret;
    if(HT->BinIterator == (HT->Bins + HT->BinsNo)) {
        HT->BinIterator = HT->Bins;
        HT->Iterator    = HT->BinIterator->HItems;
    }
    while(HT->BinIterator < (HT->Bins + HT->BinsNo)) {
        while(HT->Iterator < (HT->BinIterator->HItems + HT->BinIterator->SlotsNo )) {
            ret = *(HT->Iterator);
            HT->Iterator++;
            if(ret)
                return ret;
        }
        HT->BinIterator++;
        if(HT->BinIterator < (HT->Bins + HT->BinsNo))
            HT->Iterator = HT->BinIterator->HItems;
    }
    return NULL;
}
void HashTableResetIterator(HashTable HT) {
    HT->BinIterator = HT->Bins;
    HT->Iterator = HT->BinIterator->HItems;
}
HItem HashTableFind(HashTable HT,CHItem clone) {
    /* first get the hash key for the specific clone*/
    size_t TargetBin = HT->HashFunct(clone) % HT->BinsNo;
    HItem * TargetHItem = BinFind(&HT->Bins[TargetBin],clone,HT->compar);
    if(TargetHItem) {
        return *TargetHItem;
    }
    return NULL;
}
HItem HashTableInsert(HashTable HT,CHItem clone) {
    /* first get the hash key for the specific clone*/
    HItem ret;
    size_t TargetBin = HT->HashFunct(clone) % HT->BinsNo;
    ret = BinInsert(&HT->Bins[TargetBin],clone,HT->ctor);
    if(ret)
        HT->ItemCount++;
    return ret;
}
size_t HashTableItemCount(HashTable HT) {
    return HT->ItemCount;
}
void HashTableDelete(HashTable HT,CHItem clone) {
    /* first get the hash key for the specific clone*/
    size_t TargetBin = HT->HashFunct(clone) % HT->BinsNo;
    BinDelete(&HT->Bins[TargetBin],clone,HT->dtor,HT->compar);
    HT->ItemCount--;
}