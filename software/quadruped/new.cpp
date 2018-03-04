/*
 * New.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: gtl
 */

#include "new.hpp"

#include <stdlib.h>


void * operator new(size_t objsize) {
	return malloc(objsize);
}

void operator delete(void * obj) {
	free(obj);
}
