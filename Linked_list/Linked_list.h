//
// Created by alpharius on 22.05.2020.
//

#ifndef UNTITLED_LINKED_LIST_H
#define UNTITLED_LINKED_LIST_H

#include <string.h>
#include <zconf.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

typedef union {
    int64_t decimal;
    double real;
    uint8_t *strin;
    void* anithing;
} accepted_types;

typedef enum{
    DECIMAL,
    REAL,
    STRING,
    ANITHING
} types_t;

typedef struct LLnode {
   struct LLnode *previous;
   struct LLnode *next;

   void* key;
   void* value;
} Node;

typedef struct LinkedList{
    Node *head;
    Node *tail;
    size_t size;

    types_t key_type, value_type;
} LinkedList;


LinkedList* new_LL(types_t _key, types_t _value);
void push_LL(LinkedList* LL, size_t length, ...);
void free_LL(LinkedList* LL);
_Bool in_LL(LinkedList* LL, void* value); // поиск по значению
void print_LL(LinkedList* LL);
#endif //UNTITLED_LINKED_LIST_H
