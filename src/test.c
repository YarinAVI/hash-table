#include <stdio.h>
#include <string.h>



#include "../include/HashTable.h"
#include <stdlib.h>

int compar(CItem x, CItem y) {
    return strcmp(x,y);
}
void dtor(Item x){ 
    printf("deallocating:%s\n",(char*)x);
    free(x);
}
Item ctor(CItem x) {
    printf("allocating:%s\n",(char*)x);
    Item new = calloc(strlen(x)+1,sizeof(char));
    strcpy(new,x);
    return new;
}
size_t HashFunct(CItem x) {
    size_t result = 0;
    const char *c = x;
    while(*c) {
        result+= ( (*c) % 10 ) + 1;
        c++;
    }
    return result;
}
int main(void) {
    const char *ex[3] = {"Allah Wakbar","ISIS","     WELCOMEE    "};
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
    for(int i=0;i<3;i++) {
        char * x =HashTableFind(ht,ex[i]);
        if(x) {
            printf("Found ! %s\n",x);
        }else {
            printf("could not find %s\n",ex[i]);
        }
    }
    HashTableDelete(ht,ex[0]);
    HashTableDelete(ht,ex[1]);
    HashTableDelete(ht,ex[2]);
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