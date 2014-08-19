#ifndef UTIL_H
#define UTIL_H

#define SAFE_DELETE(ptr) if(ptr != NULL){delete ptr;ptr = NULL;}
#define SAFE_DELETE_ARRAY(ptr) if(ptr != NULL){delete[] ptr;ptr = NULL;}

#define DOUBLE_EPSILON 1e-10

#endif // UTIL_H