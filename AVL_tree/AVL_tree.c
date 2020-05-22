//
// Created by alpharius on 21.05.2020.
//

#include "AVL_tree.h"

/*
 * static block declaration
 */
static unsigned char height(Node* nptr); // внутрянняя функция
static int height_balance(Node* nptr);   // баланс высот левой и правой ветви дерева
static void count_height(Node* nptr);   // вычисляет истинную высоту родителя как max(heifht of children) + 1
static Node* rotateright(Node*);        // поворот несбалансированного дерева вправо
static Node* rotateleft(Node*);        // поворот несбалансированного дерева влево
static Node* rebalance(Node*);          // перебалансировка узла если баланс = +-2
static Node* findmin(Node*);            // поиск минимального элемента
static Node* removemin(Node*);          // удаление минимального элемента

typedef unsigned char uint_8;

Node* new_Node(int k){
    Node* result = (Node*) malloc(sizeof(Node));
    result->right = result->left = NULL;
    result->height = 1; result->key = k;
    return result;
}

Node* insert(Node* root, int k){
    if(!root) return new_Node(k);
    if(k<root->key)
        root->left = insert(root->left, k);
    else
        root->right = insert(root->right, k);
    return rebalance(root);
}

Node* remove(Node* p, int k){
    if (!p) return 0;
    if( k < p->key)
        p->left = remove(p->left, k);
    else if (k > p->key)
        p->right = remove(p->right, k);
    else {
        Node* q = p->left;
        Node* r = p->right;
        free(p);
        if (!r) return q;
        Node* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return height_balance(min);
    }
    return height_balance(p);
}

/*
 * static block definition
 */

static unsigned char height(Node* nptr){
    /* может работать с нулевыми указателями */
    return nptr? nptr->height : 0;
}

static int height_balance(Node* nptr){
    return height(nptr->right) - height(nptr->left);
}

static void count_height(Node* nptr){
    uint_8 left_height = height(nptr->left);
    uint_8 right_height = height(nptr->right);
    nptr->height = ((left_height > right_height) ? left_height : right_height) + 1;
}

static Node* rotateright(Node* p){
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    count_height(p);
    count_height(q);
    return q;
}

static Node* rotateleft(Node* q){
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    count_height(q);
    count_height(p);
    return p;
}

static Node* rebalance(Node* p){
    count_height(p);
   if(height_balance(p)==2){
       if (height_balance(p->right))
           p->left = rotateleft(p->right);
       return rotateleft(p);
   }
   if (height_balance(p)==-2){
       if(height_balance(p->left)>0)
           p->left = rotateleft(p->left);
       return rotateright(p);
   }
   return p;
}

static Node* findmin(Node* p){
    return p->left?findmin(p->left) : p;
}

static Node* removemin(Node* p){
    if(p->left==0)
        return p->right;
    p->left = removemin(p->left);
    return height_balance(p);
}