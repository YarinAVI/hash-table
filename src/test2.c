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
    const char *ex[3] = {"test1","test1","test2"};
    HashTable ht = newHashTable(ctor,dtor,compar,HashFunct);
    for(int i=0;i<3;i++) {
        HashTableInsert(ht,ex[i]);
    }
    for(int i=0;i<3;i++) {
        char * x =HashTableFind(ht,ex[i]);
        if(x) {
            printf("Found ! %s\n",x);
        }else {
            printf("could not find %s\n",ex[i]);
        }
    }
    HashTableDelete(ht,ex[1]);
    HashTableDelete(ht,ex[1]);
    for(int i=0;i<3;i++) {
        char * x =HashTableFind(ht,ex[i]);
        if(x) {
            printf("Found ! %s\n",x);
        }else {
            printf("could not find %s\n",ex[i]);
        }
    }
    HashTableDestroy(ht);
}