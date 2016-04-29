#ifndef QUAD_H__
#define QUAD_H__
#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>

#define FREE(x) if(t!=NULL) {free(t); t = NULL;} 

typedef struct str_quad
{
    int label;
    char op;
    symbol_t* arg1;
    symbol_t* arg2;
    symbol_t* res;
    struct str_quad* next;
} quad_t2;


typedef struct str_quad_list
{
  quad_t2* node;
  struct str_quad_list* next;
} quad_list_t;


quad_t2* quad_gen(int*, char, symbol_t*,symbol_t*, symbol_t*);
void         quad_add(quad_t2**,quad_t2*);
void         quad_print(quad_t2*);
void         quad_free(quad_t2*);

quad_list_t* quad_list_new(quad_t2*);
void              quad_list_add(quad_list_t**,quad_list_t*);
void              quad_list_complete(quad_list_t**,quad_t2*);
void              quad_list_print(quad_list_t*);

#endif /* end of include guard: QUAD_H__ */
