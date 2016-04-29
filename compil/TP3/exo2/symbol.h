#ifndef SYMBOL_H__
#define SYMBOL_H__


typedef struct str_symbol
{
    char *identifier;
    int isconstant;
    int value;
    struct str_symbol *next;
} symbol_t;


symbol_t* symbol_alloc();
void symbol_set(symbol_t*,char*,int,int,symbol_t*);
symbol_t* symbol_newtemp(symbol_t** table);
symbol_t* symbol_lookup(symbol_t* table, char *identifier);
symbol_t* symbol_add(symbol_t **table, char *identifier);
symbol_t* symbol_printf(symbol_t *table);

#endif /* end of include guard: SYMBOL_H__ */
