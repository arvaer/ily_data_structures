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
}

int main(){
    test_single_ops();
    test_double_ops();
    return 0;
}
