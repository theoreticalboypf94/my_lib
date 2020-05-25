//
// Created by alpharius on 22.05.2020.
//

#include "Linked_list.h"

LinkedList* new_LL(types_t _key, types_t _value){
    LinkedList* result = (LinkedList*) malloc(sizeof(LinkedList));
    result->head = NULL;
    result->tail = NULL;
    result->size = 0;
    result->key_type = _key;
    result->value_type = _value;
}

void free_LL(LinkedList* LL){
    Node* current = LL->head;
    Node* next = current->next;
    while(current){
        if (LL->key_type == STRING || LL->key_type == STRING)
            free(current->key);
        free(current);
        current = next;
        next = next->next;
    }
}

void push_LL(LinkedList* LL,size_t length, ...){
    va_list args;
    va_start(args, length);
    for(size_t i=0; i<length; i++){
        Node *llptr = (Node*) malloc(sizeof(Node));
        if(LL->head == NULL){
            LL->head = llptr;
            LL->tail = llptr;
            LL->size = 0;
            if (LL->value_type == STRING){
                char* chptr = (char*) va_arg(args, char*);
                char* alloc = (char*) malloc(strlen(chptr));
                strcpy(alloc, chptr);
                LL->head->value = alloc;
            }
            continue;
        }
        LL->tail->next = llptr;
        LL->tail = llptr;
        LL->size++;
        switch(LL->value_type){
            case STRING: {
                char *chptr = (char *) va_arg(args, char*);
                char *alloc = (char *) malloc(strlen(chptr));
                strcpy(alloc, chptr);
                LL->tail->value = alloc;
            }
                break;
        }
    }
    va_end(args);
}

void print_LL(LinkedList* LL){
    Node *llptr = LL->head;
    while(llptr){
        switch(LL->value_type){
            case STRING:
            {
                printf("=>[ %s ] ", (char*) llptr->value);
            }
        }
        llptr = llptr->next;
    }
    printf("\n");
}

_Bool in_LL(LinkedList* LL, void* value){
    Node* nptr = LL->head;
    while(nptr){
        switch (LL->value_type){
            case STRING:{
                if (strcmp((char*) value, nptr->value) == 0)
                    return 1;
            }
            break;
        }
        nptr = nptr->next;
    }
    return 0;
}