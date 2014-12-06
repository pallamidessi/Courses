#include "quad.h"
#include <stdio.h>
#include <stdlib.h>


quad_t2* quad_gen(int* current_label, char operator, symbol_t* operande_1,symbol_t* operande_2, symbol_t* result){
  
  quad_t2* new_quad=malloc(sizeof(struct str_quad));
  
  if(new_quad==NULL){
    perror("Allocation error on quad\n");
    exit(1);
  }

  //new_quad->label=*label;
  new_quad->op=operator;
  new_quad->arg1=operande_1;
  new_quad->arg2=operande_2;
  new_quad->res=result;
  new_quad->next=NULL;
  new_quad->label=*current_label;
  
  *current_label++;
  
  return new_quad;
}


void quad_add(quad_t2** quad_list, quad_t2* item){
  
  quad_t2* tmp;

  if (*quad_list==NULL) {
    *quad_list=item;
    return;
  }
  
  tmp=*quad_list;

  while (tmp->next != NULL) {
    tmp=tmp->next;
  }
  
  tmp->next=item;
}


void quad_print(quad_t2* quad){
  if (quad == NULL) {
    return;
  }
  
  while (quad != NULL) {
    
    printf("Label %d,",quad->label);
    printf("operator %c",quad->op);
    printf("%s\n",quad->arg1->identifier);
    printf("%s\n",quad->arg2->identifier);
    printf("\n");

    quad=quad->next;
  }

}


quad_list_t* quad_list_new(quad_t2* item){
  quad_list_t* list=malloc(sizeof(struct str_quad_list));

  if (list==NULL) {
    perror("Allocation error on quad_list\n");
    exit(1);
  }
  
  list->node=item;
  list->next=NULL;

  return list;
}


void quad_list_add(quad_list_t** list,quad_list_t* item){
  quad_list_t* tmp;

  if (*list==NULL) {
    *list=item;
  }
  
  tmp=*list;

  while (tmp->next != NULL) {
    tmp=tmp->next;
  }
  
  tmp->next=item;

}


void quad_list_complete(quad_list_t** list,quad_t2* to_add){
  
  quad_list_t* tmp;
  tmp=*list;

  if (list == NULL)
    return;

  if (tmp->node == NULL) {
    // create node .
  }

  while (tmp->next != NULL) {
    
    if (tmp->node!= NULL) {
      quad_add(&tmp->node,to_add);
    }
    else{
      //create node ?
    }

    tmp=tmp->next;
  }
}


void quad_list_print(quad_list_t* list){
  
  if (list == NULL)
    return;

  if (list->node!= NULL) {
    quad_print(list->node);
  }

  while (list->next != NULL) {
    
    if (list->node!= NULL) {
      quad_print(list->node);
    }

    list=list->next;
  }
  
}


