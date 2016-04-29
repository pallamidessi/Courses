#include "quad.h"
#include <stdio.h>
#include <stdlib.h>


quad_t* quad_gen(int* current_label, char operator, symbol_t* operande ,symbol_t* operande, symbol_t* result){
  
  quad_t* new_quad=malloc(sizeof(struct str_quad));
  
  if(new_quad==NULL){
    perror("Allocation error on quad\n");
    exit(1);
  }

  new_quad->label=*label;
  new_quad->op=operator;
  new_quad->arg1=operande_1;
  new_quad->arg2=operande_2;
  new_quad->res=result;
  new_quad->next=NULL;

  return new_quad;
}


void quad_add(struct quad** quad_list, struct quad* item){
  
  quad* tmp;

  if (*quad_list==NULL) {
    *quad_list=item;
  }
  
  tmp=*quad_list;

  while (tmp->next != NULL) {
    tmp=tmp->next;
  }
  
  tmp->next=item;
}


void quad_print(quad_t* quad){
  
  if (quad == NULL) {
    return;
  }
  
  printf("Label %d,",quad->label);
  printf("operator %d",quad->op);
  printf("\n");

  while (list->next != NULL) {
    
    printf("Label %d,",quad->label);
    printf("operator %d",quad->op);
    printf("\n");

    list=list->next;
  }

}


struct quad_list_t* quad_list_new(quad_t* item){
  quad_list_t* list=malloc(sizeof(struct str_quad_list));

  if (list==NULL) {
    perror("Allocation error on quad_list\n");
    exit(1);
  }
  
  list->node=item
  list->next=NULL;

  return list;
}


void quad_list_add(quad_list_t** list,quad_list_t* item){
  quad_list_t* tmp;

  if (*quad_list==NULL) {
    *quad_list=item;
  }
  
  tmp=*quad_list;

  while (tmp->next != NULL) {
    tmp=tmp->next;
  }
  
  tmp->next=item;

}


void quad_list_complete(quad_list_t* list,symbol_t* to_add){
  
  if (list == NULL)
    return;

  if (list->node == NULL) {
    // create node .
  }

  while (list->next != NULL) {
    
    if (list->node!= NULL) {
      quad_add(list->node,to_add);
    }
    else{
      //create node ?
    }

    list=list->next;
  }
}


void quad_list_print(quad_list_t* list){
  
  if (list == NULL)
    return;

  if (list->node!= NULL) {
    quad_print(list->node)
  }

  while (list->next != NULL) {
    
    if (list->node!= NULL) {
      quad_print(list->node)
    }

    list=list->next;
  }
  
}


