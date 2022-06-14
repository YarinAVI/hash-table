#include <stdio.h>
#include <string.h>



#include "../include/HashTable.h"
#include <stdlib.h>

int compar(CHItem x, CHItem y) {
    return strcmp(x,y);
}
void dtor(HItem x){ 
    printf("deallocating:%s\n",(char*)x);
    free(x);
}
HItem ctor(CHItem x) {
    printf("allocating:%s\n",(char*)x);
    HItem new = calloc(strlen(x)+1,sizeof(char));
    strcpy(new,x);
    return new;
}
size_t HashFunct(CHItem x) {
    size_t result = 0;
    const char *c = x;
    while(*c) {
        result+= ( (*c) % 10 ) + 1;
        c++;
    }
    return result;
}
int main(void) {
    const char *ex[8] = {"test1","test2","test3","Test100","too","Foo","adsadasdsad","bobo"};
    HashTable ht = newHashTable(ctor,dtor,compar,HashFunct);
    for(int i=0;i<8;i++) {
        HashTableInsert(ht,ex[i]);
    }
    size_t size  = HashTableItemCount(ht);
    for(int i=0;i<size;i++) {
        HItem it = HashTableGetNextSet(ht);
        if(it)
            printf("%s->\n",(char*)it);
    }
    HashTableResetIterator(ht);
    for(int i=0;i<size;i++) {
        HItem it = HashTableGetNextSet(ht);
        if(it)
            printf("%s->\n",(char*)it);
    }

    HashTableDestroy(ht);
return 0;
}