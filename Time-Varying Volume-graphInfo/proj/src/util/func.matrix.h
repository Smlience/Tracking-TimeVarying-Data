/************************************************************************
/* Matrix Operation
/************************************************************************/

#ifndef FUNC_MATRIX_H
#define FUNC_MATRIX_H

#include "util/Vector4.h"

extern void transposeMatrix(double* mOut, const double* rhs);
extern float invMatrix(double* mOut, const double* rhs);
extern Vector4d multiply(Vector4d p, const double* matrix);
extern void printMatrix(double* mIn, int w, int h);

#endif // FUNC_MATRIX_H