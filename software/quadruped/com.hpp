/*
 * com.hpp
 *
 *  Created on: Feb 7, 2018
 *      Author: gustavo
 */

#ifndef COM_HPP_
#define COM_HPP_

#define __sbi( reg, bit ) ((reg) |= (1 << (bit) ))
#define __cbi( reg, bit ) ((reg) &= ~(1 << (bit) ))

#define __abs( v ) ((v)<0 ? (-v) : (v))

#endif /* COM_HPP_ */
