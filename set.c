#define ILY_SET_IMPLEMENTATION
#include "ily_set.h"
#include <stdio.h>
#include <stdalign.h>
#include <openssl/evp.h>
#include <string.h>



void test_node_init(){
    Node node = {};
    int result = init_node(&node);
    assert(node.data == (unsigned char*) "");
    assert(node.height == 0);
    assert(node.parent == NULL);
    assert(node.right == NULL);
    assert(node.left == NULL);
    assert(result == SUCCESS);
}

void test_subtree_first(){
    Node A = {};
    Node B = {};
    Node C = {};
    Node D = {};
    Node E = {};

    A.left = &B;
    A.right = &C;
    B.right = &D;
    B.left = &E;

    Node* target = subtree_first(&A);
    Node* target2 = subtree_first(&B);
    Node* target3 = subtree_first(&D);
    assert(&E == target);
    assert(&E == target2);
    assert(&D == target3);
}

void test_subtree_last(){
    Node A = {};
    Node B = {};
    Node C = {};
    Node D = {};
    Node E = {};

    A.left = &B;
    A.right = &C;
    B.right = &D;
    B.left = &E;

    Node* target = subtree_last(&A);
    Node* target2 = subtree_last(&B);
    assert(&C == target);
    assert(&D == target2);
}

void test_subtree_successor(){
    Node A = {};
    Node B = {};
    Node C = {};
    Node D = {};
    Node E = {};

    A.left = &B;
    B.parent = &A;
    A.right = &C;
    C.parent = &A;
    B.right = &D;
    D.parent = &B;
    B.left = &E;
    E.parent = &B;

    Node* target = successor(&A);
    Node* target2 = successor(&B);
    assert(&C == target);
    assert(&D == target2);
}

void test_subtree_predecessor(){
    Node A = {};
    Node B = {};
    Node C = {};
    Node D = {};
    Node E = {};

    A.left = &B;
    A.right = &C;
    B.right = &D;
    B.left = &E;

    Node* target = predecessor(&A);
    Node* target2 = predecessor(&B);
    assert(&D == target);
    assert(&E == target2);
}

void test_subtree_insert_after(){
    Node A = {};
    Node B = {};
    Node C = {};
    Node D = {};
    Node E = {};

    A.left = &B;
    A.right = &C;
    B.right = &D;
    B.left = &E;

    Node target = {};
    Node target2 = {};

    int result = subtree_insert_after(&A,&target);
    assert(result);
    assert(successor(&A) ==  &target);

    result = subtree_insert_after(&A,&target2);
    assert(result);
    assert(successor(&A) == &target2);
    assert(target.left == &target2);
    assert(target2.parent == &target);
}

void test_subtree_insert_before(){
    Node A = {};
    Node B = {};
    Node C = {};
    Node D = {};
    Node E = {};

    A.left = &B;
    A.right = &C;
    B.right = &D;
    B.left = &E;

    Node target = {};
    Node target2 = {};

    int result = subtree_insert_before(&A,&target);
    assert(result);
    assert(predecessor(&A) ==  &target);

    result = subtree_insert_before(&A,&target2);
    assert(result);
    assert(predecessor(&A) == &target2);
    assert(target.right == &target2);
    assert(target2.parent == &target);
}

void test_subtree_delete(){
    Node A = {};
    Node B = {};
    Node C = {};
    Node D = {};
    Node E = {};

    A.left = &B;
    B.parent = &A;
    A.right = &C;
    C.parent = &A;
    B.right = &D;
    D.parent = &B;
    B.left = &E;
    E.parent = &B;

    int result = subtree_delete(&B);
    assert(result);
    assert(predecessor(&A) ==  &D);
    assert(A.left == &B);
    assert(B.left == NULL);

    result = subtree_delete(&B);
    assert(result);
    assert(predecessor(&B) ==  NULL);
    assert(A.left == &B);
    assert(B.right == NULL);

}


int main(){
   //test_node_init();
   //test_subtree_first();
   //test_subtree_last();
   //test_subtree_successor();
   //test_subtree_predecessor();
   //test_subtree_insert_after();
   //test_subtree_insert_before();
   test_subtree_delete();


    return 0;
}
