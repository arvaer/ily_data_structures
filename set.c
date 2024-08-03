#include "ily_set.h"
#include "stdio.h"
#include <openssl/evp.h>
#include <string.h>

unsigned char* inserter (unsigned char* str){
    int i, x, y, r;
    i = strlen((const char*) str);
    r = i % 3;
    x = i / 3;
    if(r){
        y = 5*x+2*r%4+1;
    }
    else{
        y = 4*x + 1;
    }

    unsigned char* out = (unsigned char*)malloc(y);
    if(!EVP_EncodeBlock(out, str, i)) {
        fprintf(stderr, "failure encoding");
    }
    printf("result: %s\n", out);

    return out;

}


void test_double_ops(){
    node A = {};
    A.data = inserter((unsigned char*) "Thundercock");

    node B = {};
    B.data = inserter((unsigned char*) "BaseBaller");

    node C = {};
    C.data = inserter((unsigned char*) "Nakaklor");

    node D = {};
    D.data = inserter((unsigned char*) "Peebp");

    node E = {};
    E.data = inserter((unsigned char*) "RAGO");

    node F = {};
    F.data = inserter((unsigned char*) "BABOOI");

    node G = {};
    G.data = inserter((unsigned char*) "PAONO");


    C.right = &B;
    B.left = &A;

    C.left = &D;
    D.right = &G;
    D.left = &F;
    F.left = &E;
    

    node INSERT = {};
    INSERT.data = (unsigned char*) "Fdog";

    // E F D G C B A
    subtree_insert_before(&D, &INSERT);
    // E F TARGET D G C B A
    subtree_insert_after(&D, &INSERT);
    // E F TARGET D TARGET G C B A
    printf("%s Is it correct \n", F.right->data);
    printf("%s Is it correct \n", G.left->data);
}


void test_single_ops(){
    node A = {};
    A.data = inserter((unsigned char*) "Thundercock");

    node B = {};
    B.data = inserter((unsigned char*) "BaseBaller");

    node C = {};
    C.data = inserter((unsigned char*) "Nakaklor");

    node D = {};
    D.data = inserter((unsigned char*) "Peebp");

    node E = {};
    E.data = inserter((unsigned char*) "RAGO");

    node F = {};
    F.data = inserter((unsigned char*) "BABOOI");

    node G = {};
    G.data = inserter((unsigned char*) "PAONO");


    C.right = &B;
    B.left = &A;

    C.left = &D;
    D.right = &G;
    D.left = &F;
    F.left = &E;

    //           c
    //      D        B
    //   F     G         A
    //E
    // E F D G C B A

    node* target_d = subtree_first(&D);
    assert(target_d->data == E.data);
    target_d = subtree_last(&D);
    assert(target_d->data == G.data);
    node* pred = predecessor(&B);
    node* succ = successor(&D);
    assert(pred->data == A.data);
    assert(succ->data == G.data);
}

int main(){
    test_single_ops();
    test_double_ops();
    return 0;
}
