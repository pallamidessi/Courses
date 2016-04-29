#include "symbol.h"


symbol_t* symbol_alloc(){
  symbol_t* newSymbol=calloc(1,sizeof(struct str_symbol));

  if (newSymbol == NULL) {
    perror("Allocation error on symbol\n");
    exit(1);
  }
  return newSymbol;
}

void symbol_set(symbol_t* item,char* identifier,int is_constant,int value,symbol_t* next){
  
  if (item == NULL) {
    perror("Trying to set a null pointer \n");
    exit(1);
  }

  item->identifier=identifier;
  item->isconstant=is_constant;
  item->value=value;
  item->next=next;
}

symbol_t* symbol_newtemp(symbol_t** table){
  symbol_t* tmp;
  
  if (*table == NULL) {
    *table=symbol_alloc();
    return *table;
  }
  else{
    tmp=*table;

    while (tmp->next != NULL) {
      tmp=tmp->next;
    }

    tmp->next=symbol_alloc();
    
  }
}

symbol_t* symbol_lookup(symbol_t* table, char *identifier){
  
  if (table == NULL) {
    return NULL;
  }
  
  if (strcmp(identifier,table->identifier)==0) {
    return table;
  }
  
  while (table != NULL) {

    if (strcmp(identifier,table->identifier)==0) {
      return table;
    }

    table=table->next;
  }

  return NULL;

}

symbol_t* symbol_add(symbol_t **table, symbol_t* item){
  symbol_t* tmp;
  
  if (*table == NULL) {
    *table=item;
  }
  else{
    tmp=*table;

    while (tmp->next != NULL) {
      tmp=tmp->next;
    }

    tmp->next=item;
  }
  
  return *table;
}

symbol_t* symbol_printf(symbol_t *table){
   
  if (table == NULL) {
    return;
  }
  
  while (table != NULL) {
    printf("Symbol with identifier \"%s\": \t value %d, \t and is a constant :%d \n",table->identifier,table->value, table->isconstant);
    table=table->next;
  }

}

