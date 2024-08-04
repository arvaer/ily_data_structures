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
#define ILY_SET_INSERT
#endif // ILY_SET_INSERT


typedef struct node {
    unsigned char* data;
    struct node* parent; 
    struct node* left;   // CAR
    struct node* right; // CDR
} node;


typedef struct Set{
    void* items;
    size_t size;
    size_t capacity;
} Set;


// Set API 
int initialize_set(Set* set);
int free_set(Set* set);

// Node / Tree Traversal
node* subtree_first(node* root);
node* successor(node* root);
node* predecessor(node* root);

node* subtree_insert_before(node* a, node* b);
node* subtree_insert_after(node*a, node*b);
int subtree_insert(node* a, node* b);
node* subtree_find(node* a, unsigned char* b);
node* subtree_find_next(node* a, node* b);
node* subtree_find_prev(node* a, node* b);

#endif // ILY_SET_H
#ifndef ILY_SET_IMPLEMENTATION





// Set backed by BST ops
int initialize_set(Set* set) {
    
}










// Node Traversal Ops
node* subtree_first(node* root){
    node* prev_node = NULL;
    if(!root->left){
        return root;
    }

    while(root){
        prev_node = root;
        root = root->left;
    }

    return prev_node;
}

node* subtree_last(node* root){
    node* prev_node = NULL;
    if(!root->right){
        return root;
    }

    while(root){
        prev_node = root;
        root = root->right;
    }

    return prev_node;
}


node* successor(node* root){
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

node* predecessor(node* root){
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


node* subtree_insert_before(node* a, node* b){    
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

node* subtree_insert_after(node* a, node* b){
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

node* subtree_delete(node* a) {
    if (a->left || a->right) {
        node* b = NULL;
        if(a->left){
            b = predecessor(a);
        } else {
            b = successor(a);
        }
        
        node* temp = a;
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


node* subtree_find(node* a, unsigned char* b){
    if(a == NULL) {
        return NULL;
    }

    if (a->data == b){
        return a;
    }
    else if (a->data < b) {
        subtree_find(a->left, b);
    } 
    else if (a->data > b) {
        subtree_find(a->right, b);
    }

    return NULL;
}



#define ILY_SET_IMPLEMENTATION
#endif // ILY_SET_IMPLEMENTATION
