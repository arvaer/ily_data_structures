#ifndef ILY_SET_H
#define ILY_SET_H
#include <math.h>

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
    unsigned char* decode_string (unsigned char* str);
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
    unsigned char* data; //8
    size_t height; //8
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct Set{
    Node* root;
    size_t capacity;
    size_t size;
} Set;



// Set API
extern int initialize_set(Set* set);
extern int free_set(Set* set);
extern int insert(Set* set, const char* item);
extern int remove_item(Set* set, const char* item);
extern void dump_set(Set* set);
extern int height(Set* set);
extern const char* pop(Set* set);
extern int filter(Set* in, Set* out, const char* pattern);
extern Set* build(Set* set, Node** items);
extern int disjoint_union(Set* out, Set* s1, Set* s2);
extern int collapse(Set* out, Set** sets);

#ifdef ILY_SET_IMPLEMENTATION

#define SWAP_NODES(a, b) \
        Node temp; \
        temp.data    = a->data; \
        a->data      = b->data; \
        b->data      = temp.data; \

#define SWAP_FULL_NODES(a, b) \
        Node temp; \
        temp.data    = a->data; \
        a->data      = b->data; \
        b->data      = temp.data; \
        temp.parent    = a->parent; \
        a->parent      = b->parent; \
        b->parent      = temp.parent; \
        temp.left    = a->left; \
        a->left      = b->left; \
        b->left      = temp.left; \
        temp.right    = a->right; \
        a->right      = b->right; \
        b->right      = temp.right; \


// Node API


int init_node(Node* node);
Node* subtree_first(Node* root);
Node* subtree_last(Node* root);

Node* successor(Node* root);
Node* predecessor(Node* root);

int subtree_insert_after(Node* a, Node* b);
int subtree_insert_before(Node* a, Node* b);
int subtree_insert(Node* a, Node* b);
int subtree_delete(Node* root);

#define SUCCESS 1
#define FAIL 0

int init_node(Node* node){
    if (!node) return FAIL;

    memset(node, 0, sizeof(Node));
    node->data = (unsigned char*)""; //8

    return SUCCESS;
}

Node* subtree_first(Node* root) {
    if(!root){
        return NULL;
    }
    while(root->left){
        root = root->left;
    }
    return root;
}

Node* subtree_last(Node* root) {
    if(!root){
        return NULL;
    }
    while(root->right){
        root = root->right;
    }
    return root;
}

Node* successor(Node* node){
    if(!node){
        return NULL;
    }
    if(node->right) return subtree_first(node->right);
    while(node->parent && node->parent->right == node) node = node->parent;
    return node->parent;
}

Node* predecessor(Node* node){
    if(!node){
        return NULL;
    }
    if(node->left) return subtree_last(node->left);
    while(node->parent && node->parent->left == node) node = node->parent;
    return node->parent;
}

int subtree_insert_after(Node* a, Node* b) {
    if(!a || !b) {
        return FAIL;
    }
    if(!a->right){
        a->right = b;
        b->parent = a;
        return SUCCESS;
    }
    else{
        a = subtree_first(a->right);
        a->left = b;
        b->parent = a;
        return SUCCESS;
    }
    return FAIL;
}

int subtree_insert_before(Node* a, Node* b) {
    if(!a || !b) {
        return FAIL;
    }
    if(!a->left){
        a->left = b;
        b->parent = a;
        return SUCCESS;
    }
    else{
        a = subtree_last(a->left);
        a->right = b;
        b->parent = a;
        return SUCCESS;
    }
    return FAIL;
}

int subtree_insert(Node* a, Node* b){
    if(!a || !b) return FAIL;
    unsigned char* a_data = a->data;
    unsigned char* b_data = b->data;
    
    if(!a_data || !b_data) return FAIL;

    if(gt(a_data, b_data)) {
        while(a->left){
            a = a->left;
     }
        return subtree_insert_before(a, b);
    }
    else if(lt(a_data,b_data)) {
        while(a->right){
            a = a->right;
        }
        return subtree_insert_after(a, b);
    }
    else{
        a->data = b->data;
        return SUCCESS;
    }
    return FAIL;
}

int subtree_delete(Node* root){
    if(!root) return FAIL;
    while(root){
        if(root->left || root->right){
            Node* sub;
            if(root->left) sub = predecessor(root);
            else sub = successor(root);
            SWAP_NODES(root, sub);
            root = sub;
        }
        else break;
    }
    if(root->parent){
        Node* left = root->parent->left;
        if(left == root) {
            free(left->data);
            root->parent->left = NULL;
        } else{
            Node* right = root->parent->right;
            free(right->data);
            root->parent->right = NULL;
        }
        return SUCCESS;
    }
    return FAIL;
}




// helper functions
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

unsigned char* decode_string (unsigned char* str){
    int i, x, y, r;
    i = strlen((const char*) str);
    r = i % 4;
    x = i / 4;
    if(r){
        y = 4*x+2*r%4+1;
    }
    else{
        y = 3*x + 1;
    }

    unsigned char* out = (unsigned char*)malloc(y);
    if(!EVP_DecodeBlock(out, str, i)) {
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
#endif // ILY_SET_IMPLEMENTATION
#endif // ILY_SET_H
