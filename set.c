#include "ily_set.h"
#include <stdio.h>
#include <stdalign.h>
#include <openssl/evp.h>
#include <string.h>


void test_double_ops(){
}


void test_single_ops(){
    Set set;
    initialize_set(&set);

    insert(&set, "A");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "B");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "C");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "D");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "E");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "F");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "D");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "H");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "I");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "J");
    dump_set(&set);
    printf("\n\n\n");
    insert(&set, "K");
    dump_set(&set);
    printf("\n\n\n");

    const char* item = pop(&set);
    printf("the item: %s\n", item);
    
    item = pop(&set);
    printf("the item: %s\n", item);

    item = pop(&set);
    printf("the item: %s\n", item);
    item = pop(&set);
    printf("the item: %s\n", item);
    item = pop(&set);
    printf("the item: %s\n", item);
}

int main(){
    test_single_ops();
    test_double_ops();


    return 0;
}
