#include "ily_set.h"
#include "stdio.h"
#include <openssl/evp.h>
#include <string.h>


void test_double_ops(){
}


void test_single_ops(){
    Set set;
    initialize_set(&set);

    insert(&set, "Hello world!");
    insert(&set, "is this working");
    insert(&set, "Test again");

    insert(&set, "Dunking wagon!");
    insert(&set, "is tking");
    insert(&set, "Tesasdfad again");
    insert(&set, "Tesasdfad aFISASDFK");
    insert(&set, "Tesasa Traogn");
    insert(&set, "Thunkder  h");
    insert(&set, "Sometihng again");
    insert(&set, "TesDragonn again");
    insert(&set, "gargba again");
    insert(&set, "gargba skahdf");
    insert(&set, "gargba skahdf");
    insert(&set, "This has a lot more things onw");
    insert(&set, "Sometihng in the waterore things onw");
    remove_item(&set, "Test again");
}

int main(){
    test_single_ops();
    test_double_ops();
    return 0;
}
