#ifndef QUAD_H__
#define QUAD_H__

#define FREE(x) if(t!=NULL) {free(t); t = NULL;} 

typedef struct str_quad
{
    int label;
    char op;
    struct symbol *arg1;
    struct symbol *arg2;
    struct symbol *res;
    struct quad *next;
} quad_t;


typedef struct str_quad_list
{
  struct quad_t* node;
  struct quad_list_t* next;
} quad_list_t;


quad_t* quad_gen(int*, char, symbol_t*,symbol_t*, symbol_t*);
void         quad_add(struct quad*, struct quad*);
void         quad_print(struct quad*);
void         quad_free(struct quad*);

struct quad_list* quad_list_new(quad_t*);
void              quad_list_add(quad_list_t**,quad_list_t*);
void              quad_list_complete(quad_list_t*,symbol_t*);
void              quad_list_print(quad_list_t*);

#endif /* end of include guard: QUAD_H__ */
