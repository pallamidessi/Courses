#ifndef __GAUSS_H_
#define __GAUSS_H_
#include"tp1.h"

Matrix permuter(Matrix m, int row1,int row2);
Matrix multLigne(Matrix m,int row,float scalar);
Matrix addMultLigne(Matrix m,int row1,int row2,float scalar);
Matrix Pivot_Gauss(Matrix m);
int m_determinant(Matrix m);

#endif
