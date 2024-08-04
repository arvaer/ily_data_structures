#ifndef ILY_SET_H
#define ILY_SET_H

#ifndef ILY_SET_MALLOC
#include <stdlib.h>
#define ILY_SET_MALLOC malloc 
#endif                   

#ifndef ILY_SET_ASSERT
#include <assert.h>
#define ILY_SET_ASSERT assert
#endif // ILY_SET_ASSERT

#ifndef ILY_SET_INSERT
#include <openssl/evp.h>
#include <string.h>
unsigned char* encode_string (unsigned char* str);
#define ILY_SET_INSERT
#endif // ILY_SET_INSERT

#ifndef ILY_SET_COMPARE
#include <openssl/evp.h>
#include <string.h>
int lt(unsigned char* s1, unsigned char* s2);
int leq(unsigned char* s1, unsigned char* s2);
int gt(unsigned char* s1, unsigned char* s2);
int geq(unsigned char* s1, unsigned char* s2);
int eq(unsigned char* s1, unsigned char* s2);

#define ILY_SET_COMPARE
#endif // ILY_SET_COMPARE

typedef struct Node {
    unsigned char* data;
    struct Node* parent; 
    struct Node* left;   // CAR
    struct Node* right; // CDR
} Node;


typedef struct Set{
    Node** items;
    Node* root;
    size_t capacity;
    size_t size;
} Set;


// Set API 
int initialize_set(Set* set);
int free_set(Set* set);
int insert(Set* set, const char* item);
int remove_item(Set* set, const char* item);

// Node / Tree Traversal
Node* subtree_first(Node* root);
Node* successor(Node* root);
Node* predecessor(Node* root);

Node* subtree_insert(Node* a, Node* b);
Node* subtree_insert_before(Node* a, Node* b);
Node* subtree_insert_after(Node*a, Node*b);
Node* subtree_find(Node* a, unsigned char* b);
Node* subtree_find_next(Node* a, unsigned char* b);
Node* subtree_find_prev(Node* a, unsigned char* b);
Node* subtree_delete(Node* a);

#endif // ILY_SET_H
#ifndef ILY_SET_IMPLEMENTATION


// Set backed by BST ops
int initialize_set(Set* set) {
    set->items = NULL;
    set->size = 0;
    set->capacity = 0;
    return 1;
}

int insert(Set* set, const char* item){
    unsigned char* u_item = (unsigned char*) item; //static cast
    unsigned char* encoded_string = encode_string(u_item);

    Node* node = (Node*)malloc(sizeof(Node));
    node->data = encoded_string;
   
    if(set->size >= set->capacity) {
        if(set->capacity == 0) {
           set->root = node;
           set->capacity = 5;
        } else {
            set->capacity = set->size * 2;
        }
        
        Node** new_items = (Node**)realloc(set->items, set->capacity * sizeof(Node*));
        if (!new_items) {
            return 0;
        } 
        set->items = new_items;
    }
    subtree_insert(set->root, node);
    set->items[set->size++] = node;
    return 1;
}

int remove_item(Set* set, const char* item) {
    unsigned char* u_item = (unsigned char*) item; //static cast
    unsigned char* encoded_string = encode_string(u_item);

    Node* target = subtree_find(set->root, encoded_string);
    if(target) {
        subtree_delete(target);
        if(target->parent) {
            set->root = NULL; 
        }

        free(target);
        target = NULL;
        return 1;
    }

    return 0;
}

// Node Traversal Ops
Node* subtree_first(Node* root){
    Node* prev_node = NULL;
    if(!root->left){
        return root;
    }

    while(root){
        prev_node = root;
        root = root->left;
    }

    return prev_node;
}

Node* subtree_last(Node* root){
    Node* prev_node = NULL;
    if(!root->right){
        return root;
    }

    while(root){
        prev_node = root;
        root = root->right;
    }

    return prev_node;
}


Node* successor(Node* root){
    if(root->right){
        return subtree_first(root->right);
    }
    while(root->parent){
        if (root == root->parent->left){
            return root;
        }
        root = root->parent;
    }
    return root;
};

Node* predecessor(Node* root){
    if(root->left){
        return root->left;
    }
    while(root->parent){
        if (root == root->parent->right){
            return root;
        }
        root = root->parent;
    }
    return root;
};


Node* subtree_insert(Node* a, Node* b){
   if ((a == NULL || b == NULL) && (a->data == b->data)) {
       return NULL;
   }
    if(lt(b->data, a->data)){
        if(a->left){
            subtree_insert(a->left, b);
        } else {
            subtree_insert_before(a,b);
        }
    }
    else if (gt(b->data , a->data)){
        if(a->right){
            subtree_insert(a->right, b);
        } else {
            subtree_insert_after(a,b);
        }
    }
    else {
        a->data = b->data;
    }

    return NULL;
}

Node* subtree_insert_before(Node* a, Node* b){    
    if(a->left){
        a = subtree_last(a->left);
        a->right = b;
        b->parent = a;
    }
    else{
        a->left = b;
        b->parent = a;
    }
    return NULL;
}

Node* subtree_insert_after(Node* a, Node* b){
    if(a->right){
        a = subtree_first(a->right);
        a->left = b;
        b->parent = a;
    }
    else{
        a->right = b;
        b->parent = a;
    }

    return NULL;
}

Node* subtree_delete(Node* a) {
    if (a->left || a->right) {
        Node* b = NULL;
        if(a->left){
            b = predecessor(a);
        } else {
            b = successor(a);
        }
        
        Node* temp = a;
        a->data = b->data;
        b->data = temp->data;
        subtree_delete(b);
    }

    if (a->parent){
       if (a->parent->left == a){
            a->parent->left = NULL;
       }else{
            a->parent->right = NULL;
       }
    }
    return a;
}


Node* subtree_find(Node* a, unsigned char* b){
    if(a == NULL) {
        return NULL;
    }

    if (a->data == b){
        return a;
    }

    while(a){
        if (gt(a->data ,b)) {
            a = a->left;
        } 
        else if (lt(a->data , b)) {
            a = a->right;
        }

        else {
            return a;
        }
    }

    return NULL;
}

Node* subtree_find_next(Node* a, unsigned char* b) {
    if (a == NULL) {
        return NULL;
    }
    while(a){
        if(leq(a->data, b)){
            if(a->right){
                a = a->right;
            }
            else{
                return NULL;
            }
        }
        if(a->left) {
            a = a->left;
        }
        return a;
    }
    return NULL;
}

Node* subtree_find_prev(Node* a, unsigned char* b) {
    if (a == NULL) {
        return NULL;
    }
    while(a){
        if(geq(a->data, b)){
            if(a->left){
                a = a->left;
            }
            else{
                return NULL;
            }
        }
        if(a->left) {
            a = a->left;
        }
        return a;
    }
    return NULL;
}

unsigned char* encode_string (unsigned char* str){
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

    return out;
}

int lt(unsigned char* us1, unsigned char* us2){
    const char* s1 = (const char *)us1;
    const char* s2 = (const char *)us2;

    if(s1  == NULL || s2 == NULL){
        return 0;
    }
    int result = strcmp(s1, s2);
    if (result < 0) return 1;

    return 0;
}
int leq(unsigned char* us1, unsigned char* us2){
    const char* s1 = (const char *)us1;
    const char* s2 = (const char *)us2;

    if(s1  == NULL || s2 == NULL){
        return 0;
    }
    int result = strcmp(s1, s2);
    if (result <= 0) return 1;

    return 0;
}

int gt(unsigned char* us1, unsigned char* us2){
    const char* s1 = (const char *)us1;
    const char* s2 = (const char *)us2;

    if(s1  == NULL || s2 == NULL){
        return 0;
    }
    int result = strcmp(s1, s2);
    if (result > 0) return 1;

    return 0;
}
int geq(unsigned char* us1, unsigned char* us2){
    const char* s1 = (const char *)us1;
    const char* s2 = (const char *)us2;

    if(s1  == NULL || s2 == NULL){
        return 0;
    }
    int result = strcmp(s1, s2);
    if (result >= 0) return 1;

    return 0;
}
int eq(unsigned char* us1, unsigned char* us2){
    const char* s1 = (const char *)us1;
    const char* s2 = (const char *)us2;

    if(s1  == NULL || s2 == NULL){
        return 0;
    }
    int result = strcmp(s1, s2);
    if (result == 0) return 1;

    return 0;
}
#define ILY_SET_IMPLEMENTATION
#endif // ILY_SET_IMPLEMENTATION
