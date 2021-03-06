/**
 * @file Intersection_mask.c
 * @author Pallamidessi Joseph
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
**/ 
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

typedef int bool;

typedef struct
{
  float x, y, z;
} Vector;

/*Create 4 bits bitmap using an unsigned char, 1 if the parameter is !=0, else 0 
 * Bitmaps are used to avoid numerous boolean operator.
 */

unsigned char create_mask(float f1,float f2,float f3,float f4){
  unsigned char mask=0;
  unsigned char tmp=1;
  if (f1) {
    mask=mask|(tmp<<3);
    tmp=1;
  }
  if (f2) {
    mask=mask|(tmp<<2);
    tmp=1;
  }
  if (f3) {
    mask=mask|(tmp<<1);
    tmp=1;
  }
  if (f4) {
    mask=mask|(tmp);
    tmp=1;
  }

  return mask;
}

/*Create a new Vector object*/
Vector V_new(float x, float y, float z)
{
  Vector v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

/*Produit scalaire*/
float V_dot(Vector v1, Vector v2){
  float result=0;

  result+=v1.x*v2.x;
  result+=v1.y*v2.y;
  result+=v1.z*v2.z;

  return result;
}

Vector V_substract(Vector v1, Vector v2){
  Vector new=V_new(0,0,0);

  new.x=v1.x-v2.x;
  new.y=v1.y-v2.y;
  new.z=v1.z-v2.z;

  return new;

}
float det_from_vectors(Vector v1,Vector v2,Vector v3){
  return  ((v1.x*v2.y*v3.z 
          + v2.x*v1.z*v3.y 
          + v1.y*v2.z*v3.x) 
          - (v1.z*v2.y*v3.x 
          + v1.y*v2.x*v3.z 
          + v1.x*v3.y*v2.z) );
}

bool are_segments_intersected(Vector p1,Vector p2,Vector q1,Vector q2,unsigned char mask[4]){
  float s1,s2,s3,s4;
  unsigned char mask_det;

  s1=det_from_vectors(p1,p2,q1);
  s2=det_from_vectors(p1,p2,q2);
  s3=det_from_vectors(q1,q2,p1);
  s4=det_from_vectors(q1,q2,p2);
  mask_det=create_mask(s1,s2,s3,s4);

  /*Cas simple*/
  if (s1*s2<0 && s3*s4<0) {
    return TRUE;
  }
  /*vecteur directeur colinéaire*/
  else if(!mask_det ){
    //q1 entre p1 et p2
    if(V_dot(V_substract(p1,q1),V_substract(p2,q1))<=0)
      return TRUE;
    //q2 entre p1 et p2
    if(V_dot(V_substract(p1,q2),V_substract(p2,q2))<=0)
      return TRUE;
    //p1 entre q1 et q2
    if(V_dot(V_substract(q1,p1),V_substract(q2,p1))<=0)
      return TRUE;
    //p2 entre q1 et q2
    if(V_dot(V_substract(q1,p2),V_substract(q2,p2))<=0)
      return TRUE;
  }
  /*intersection en q1*/
  else if (mask_det==mask[0]) {
    if(V_dot(V_substract(p1,q1),V_substract(p2,q1))<=0)
      return TRUE;
  }
  /*intersection en q2*/

  else if (mask_det==mask[1]) {
    if(V_dot(V_substract(p1,q2),V_substract(p2,q2))<=0)
      return TRUE;
  }
  /*intersection en p1*/
  else if (mask_det==mask[2]) {
    if(V_dot(V_substract(q1,p1),V_substract(q2,p1))<=0)
      return TRUE;
  }
  /*intersection en p2*/
  else if (mask_det==mask[3]) {
    if(V_dot(V_substract(q1,p2),V_substract(q2,p2))<=0)
      return TRUE;
  }
  
  return FALSE;
}

/*Z field must be set to atleast 1, for the determinant to be correct*/
int main(int argc, char* argv[]){
  unsigned char mask[4];
  
  if (argc !=13) {
    printf("usage: %s p1 p2 q1 q2 (Vector) \n",argv[0]);
    exit(1);
  }
  /*Points creation*/
  Vector p1=V_new(atof(argv[1]),atof(argv[2]),atof(argv[3]));
  Vector p2=V_new(atof(argv[4]),atof(argv[5]),atof(argv[6]));
  Vector q1=V_new(atof(argv[7]),atof(argv[8]),atof(argv[9]));
  Vector q2=V_new(atof(argv[10]),atof(argv[11]),atof(argv[12]));

  /*Creation of the 4 different bitmap, each corresponding to 1  
   * determinant equal 0 and the 3 others differents from 0*/
  mask[0]=create_mask(0,1,1,1);
  mask[1]=create_mask(1,0,1,1);
  mask[2]=create_mask(1,1,0,1);
  mask[3]=create_mask(1,1,1,0);
  
  if (are_segments_intersected(p1,p2,q1,q2,mask)==TRUE) {
    printf("Les segments s'intersectent.\n");
  }
  else {
    printf("Les segments ne se coupent pas.\n");
  }

  return 0;
}
