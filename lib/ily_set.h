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


#define SWAP_NODES(a, b) \
        Node temp; \
        temp.data    = a->data; \
        a->data      = b->data; \
        b->data      = temp.data; \

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
int initialize_set(Set* set);
int free_set(Set* set);
int insert(Set* set, const char* item);
int remove_item(Set* set, const char* item);
int height(Set* set);
const char* pop(Set* set);
int filter(Set* in, Set* out, const char* pattern);
Set* build(Set* set, Node** items);
int disjoint_union(Set* out, Set* s1, Set* s2);
int collapse(Set* out, Set** sets);
const char* min(Set* set);
const char* max(Set* set);



// Node / Tree Traversal
Node* successor(Node* root);
Node* predecessor(Node* root);

Node* subtree_first(Node* root);
Node* subtree_insert(Node* a, Node* b);
Node* subtree_insert_before(Node* a, Node* b);
Node* subtree_insert_after(Node*a, Node*b);
Node* subtree_find(Node* a, unsigned char* b);
Node* subtree_find_next(Node* a, unsigned char* b);
Node* subtree_find_prev(Node* a, unsigned char* b);
Node* subtree_delete(Node* a);
Node* subtree_maintain(Node* a);
Node* subtree_rotate_left(Node* a);
Node* subtree_rotate_right(Node* a);

#endif // ILY_SET_H
#ifndef ILY_SET_IMPLEMENTATION
// Set backed by BST ops
int initialize_set(Set* set) {
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
        
    }
    subtree_insert(set->root, node);
    set->size++;
    set->root = subtree_maintain(node);
    return 1;
}

int remove_item(Set* set, const char* item) {
    unsigned char* u_item = (unsigned char*) item; //static cast
    unsigned char* encoded_string = encode_string(u_item);

    Node* target = subtree_find(set->root, encoded_string);
    if(target) {
        subtree_delete(target);
        if(!target->parent) {
            set->root = NULL; 
        }else{
            subtree_maintain(target->parent);
        }

        free(target);
        target = NULL;
        set->size--;
        return 1;
    }

    return 0;
}

const char* pop(Set* set) {
    if(!set) {
        fprintf(stderr, "No set \n");
        return NULL;
    }
    if(!set->root) {
        fprintf(stderr, "No set root \n");
        return NULL;
    }
    Node* target = subtree_first(set->root);
    if(target) {
        char* decoded_data = (char*)decode_string(target->data);
        char* buf = malloc(sizeof(buf) * strlen(decoded_data));
        stpcpy(buf, decoded_data);

        Node* parent = target->parent;
        subtree_delete(target);
        if(!parent) {
            set->root = NULL; 
        }else{
            set->root = subtree_maintain(parent);
        }

        free(target);
        target = NULL;
        return buf;
    }

    return NULL;
}

int height(Set* set) {
    if(!set){
        fprintf(stderr, "Function called with null set");

    }
    if(!set->root){
        fprintf(stderr, "Function called with null root");

    }
    return set->root->height;
}


// Node Traversal Ops
size_t calculate_height(Node* a){
    if(!a->left && !a->right){
        return 1;
    }
    else if(a->left && !a->right){
        return a->left->height + 1;
    }
    else if (!a->left && a->right){
        return a->right->height + 1;
    }
    else {
        double left_h = (double)a->left->height;
        double right_h = (double)a->right->height;
        
        return fmax(left_h, right_h);
    }
}

int calculate_skew(Node* a) {
    size_t right = 0;
    size_t left = 0;
    if(a->right) {
        right = calculate_height(a->right);
    }
    if(a->left) {
        left = calculate_height(a->left);
    }

    return right - left;
}

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
   if ((a == NULL || b == NULL) && (eq(a->data, b->data))) {
       return NULL;
   }
    if(lt(b->data, a->data)){
        subtree_insert_before(a,b);
    }
    else if (gt(b->data , a->data)){
        subtree_insert_after(a,b);
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
        
        SWAP_NODES(a,b);
        subtree_delete(b);
    }

    if (a->parent){
       if (a->parent->left == a){
            a->parent->left = NULL;
       }else{
            a->parent->right = NULL;
       }
    }

    free(a->data);
    a->data = NULL;
    return a;
}


Node* subtree_find(Node* a, unsigned char* b){
    if(a == NULL) {
        return NULL;
    }

    if (eq(a->data, b)){
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

Node* subtree_rotate_right(Node* d){
    if(!d->left){
        fprintf(stderr, "rotation failed: d.left not present");
        return NULL;
    }

    Node* b = d->left;
    Node* e = d->right;
    Node* a = b->left;
    Node* c = b->right;

    SWAP_NODES(b,d);
    if(d->parent){
        if(d->parent->right == d){
            d->parent->right = b;
        }
        else{
            d->parent->left = b;
        }
        b->parent = d->parent;
    }
    else{
        b->parent = NULL;
    }


    b->left = a;
    if(a) a->parent = b;

    b->right = d;
    if(d) d->parent = b;

    d->left = c;
    if(c) c->parent = d;
    d->right = e;
    if(e) e->parent = d;

    d->height = calculate_height(d);
    b->height = calculate_height(b);
    return b;

}

Node* subtree_rotate_left(Node* b){
    if(!b->right){
        fprintf(stderr, "rotation failed: b.right not present");
        return NULL;
    }

    Node* a = b->left;
    Node* d = b->right;
    Node* c = d->left;
    Node* e = d->right;

    SWAP_NODES(b,d);
    if(b->parent){
        if(b->parent->right == b){
            b->parent->right = d;
        }
        else{
            b->parent->left = d;
        }
        d->parent = b->parent;
    }
    else{
        d->parent = NULL;
    }

    d->left = b;
    if(b) e->parent = d;

    d->right = e;
    if(e) e->parent = d;

    b->left = a;
    if(a) a->parent = b;

    b->right = c;
    if(c) c->parent = b;

    b->height = calculate_height(b);
    d->height = calculate_height(d);

    return d;
}



Node* subtree_rebalance(Node* a){
    int skew = calculate_skew(a);
    if(skew == 2){
        if(a->right) {
            int r_skew = calculate_skew(a->right);
            if(r_skew < 0) return subtree_rotate_right(a->right);
        }
        return subtree_rotate_left(a);
    }
    if(skew == -2) {
        if(a->left){
            int l_skew = calculate_skew(a->left);
            if(l_skew > 0) return subtree_rotate_left(a->left);
        }
        return subtree_rotate_right(a);
    }

    return a;
}

Node* subtree_maintain(Node* a) {
    while(a) {
        a = subtree_rebalance(a);
        a->height = calculate_height(a);
        if(a->parent) {
            a = a->parent;
        } else {
            break;
        }
    }
    return a;
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
#define ILY_SET_IMPLEMENTATION
#endif // ILY_SET_IMPLEMENTATION
