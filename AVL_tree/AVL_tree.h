//
// Created by alpharius on 21.05.2020.
//

#ifndef AVL_TREE_AVL_TREE_H
#define AVL_TREE_AVL_TREE_H

#include <zconf.h>
#include <stdlib.h>

typedef struct node_s {
    int key;
    struct node_s *left;
    struct node_s *right;
    unsigned char height;
} Node;

Node* new_Node(int);
Node* insert(Node* root, int k); // вставка ключа в дерево с корнем p
Node* remove(Node* p, int k);   // удаление по ключу

#endif //AVL_TREE_AVL_TREE_H
