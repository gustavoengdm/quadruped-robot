/*
 * New.hpp
 *
 *  Created on: Oct 20, 2013
 *      Author: gtl
 */

#ifndef NEW_HPP_
#define NEW_HPP_

#include <stddef.h>

void * operator new(size_t objsize);
void operator delete(void * obj);

#endif /* NEW_HPP_ */
