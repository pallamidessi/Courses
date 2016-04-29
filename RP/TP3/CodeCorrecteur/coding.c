/**
 * @file coding.c
 * @author Arash Habibi
 * @author Julien Montavont
 * @version 2.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Generate code words from the initial data flow
 */

#include "codingdecoding.h"

void copyDataBitsCoding(char *message, CodeWord_t *cw, int size)
{
  int i = 0;

  for(i=0; i<size; i++)
    {
      setNthBitCW(&(cw[i]), 1, getNthBit(message[i], 1));
      setNthBitCW(&(cw[i]), 2, getNthBit(message[i], 2));
      setNthBitCW(&(cw[i]), 3, getNthBit(message[i], 3));
      setNthBitCW(&(cw[i]), 4, getNthBit(message[i], 4));
      setNthBitCW(&(cw[i]), 5, getNthBit(message[i], 5));
      setNthBitCW(&(cw[i]), 6, getNthBit(message[i], 6));
      setNthBitCW(&(cw[i]), 7, getNthBit(message[i], 7));
      setNthBitCW(&(cw[i]), 8, getNthBit(message[i], 8));
    }

  return;
}


/*
void computeCtrlBits(CodeWord_t *message, int size)
{
	int i,j;
	unsigned int bit=0;

 	for(i=0;i<size/(int) sizeof(CodeWord_t);i++){
		for(j=1;j<=8;j++){		
			bit+=getNthBit(message[i],j);
			bit%=2;
		}
 		setNthBitCW(&message[i],9,bit);
	}
	return;
}
*/

void computeCtrlBits(CodeWord_t *message, int size)
{
	int i = 0;	
	int j = 0;	
	int k = 0;	

	int X[9];
	
	for(i=0;i<9;i++){
		X[i]=0;
	}

	CodeWord_t* toCompute;
	int entree=0;

	for(i=0;i<size/(int) sizeof(CodeWord_t);i++){
		toCompute=&message[i];
		
		for(k=0;k<9;k++){
			X[k]=0;
		}

		for(j=0;j<8;j++){
			entree=getNthBit(*toCompute,8+j);

			X[8]=X[7];		
			X[7]=X[6];		
			X[6]=X[5];		
			X[5]=X[4];		
			X[4]=X[3]^X[8];		
			X[3]=X[2]^X[8];		
			X[2]=X[0]^X[8];		
			X[1]=X[0];		
			X[0]=entree^X[8];		
		}
		for(j=0;j<8;j++){
			setNthBitCW(toCompute,9+j,X[j]);	
		}
	}
	
	return;
}

void coding(char *message, int data_size, char *cw, int *cw_size)
{
  *(cw_size)= data_size * sizeof(CodeWord_t);
	//int i;

  copyDataBitsCoding(message, (CodeWord_t*)cw, data_size);
	
	/*
		for(i=0;i<data_size-1;i++){
			printBits(cw[i],"");
		}
	*/

  //-- to uncomment when complete and needed
  computeCtrlBits((CodeWord_t*)cw, *cw_size);

  return;
}
