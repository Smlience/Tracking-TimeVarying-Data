#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "util/func.color.h"
#include "util/func.matrix.h"

extern double calcImageEntropy(const unsigned char* pixels, int w, int h, int bpp);

template<class T>
float distance(T v00, T v01, T v02, T v10, T v11, T v12){
	T v1 = v00 - v10;
	T v2 = v01 - v11;
	T v3 = v02 - v12;
	return sqrt(v1 * v1 + v2 * v2 + v3 * v3);
};

#endif // FUNCTIONS_H