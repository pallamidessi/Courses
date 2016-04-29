/**
 * @file decoding.c
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
 * Functions to decipher the code words
 */

#include <stdio.h>
#include "codingdecoding.h"

void copyDataBitsDecoding(CodeWord_t *cw, char *message, int data_size)
{
	int i = 0;

	for(i=0; i<data_size; i++)
	{
		setNthBitW(&(message[i]), 1, getNthBit(cw[i], 1));
		setNthBitW(&(message[i]), 2, getNthBit(cw[i], 2));
		setNthBitW(&(message[i]), 3, getNthBit(cw[i], 3));
		setNthBitW(&(message[i]), 4, getNthBit(cw[i], 4));
		setNthBitW(&(message[i]), 5, getNthBit(cw[i], 5));
		setNthBitW(&(message[i]), 6, getNthBit(cw[i], 6));
		setNthBitW(&(message[i]), 7, getNthBit(cw[i], 7));
		setNthBitW(&(message[i]), 8, getNthBit(cw[i], 8));
	}
}

int thereIsError(CodeWord_t *cw, int data_size)
{
	int i = 0;	
	int j = 0;	
	int k = 0;	
	int error=1;

	int X[9];
	
	for(i=0;i<9;i++){
		X[i]=0;
	}

	CodeWord_t* toTest;
	int entree=0;

	for(i=0; i<data_size; i++){
		toTest=&cw[i];
		
		for(k=0;k<9;k++){
			X[k]=0;
		}

		for(j=0;j<8;j++){
			entree=getNthBit(*toTest,8+j);

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
			if(X[j]!=getNthBit(*toTest,9+j))
				return error;
		}
	}
	
	return !error;
}

/**
 * Up to you

 void errorCorrection(CodeWord_t *cw, int data_size)
 {
 return;
 }
 */

/*
int thereIsError(CodeWord_t *cw, int data_size)
{
	int i,j;
	int error=0;
	int bit=0;


	for(i=0;i<data_size;i++){
		for(j=1;j<=8;j++){		
			bit+=getNthBit(cw[i],j);
			bit%=2;
		}

		if(bit!=getNthBit(cw[i],9)){
			error=1;;
		}
	}


	return error;
}
*/

void decoding(char *cw, int cw_size, char *message, int *data_size)
{
	*data_size = cw_size / sizeof(CodeWord_t);

	//-- For error correction
	//-- to uncomment when complete and needed
	//errorCorrection((CodeWord_t*)cw, *data_size);

	//-- For decoding
	copyDataBitsDecoding((CodeWord_t*)cw, message, *data_size);

	//-- For error detection
	//-- to uncomment when complete and needed
	if(thereIsError((CodeWord_t*)cw, *data_size))
	{
		printf("PARITY ERROR: \"%s\"\n", message);
	}

	return;
}
