/*
 * robot.hpp
 *
 *  Created on: Mar 9, 2018
 *      Author: gustavo
 */

#ifndef ROBOT_HPP_
#define ROBOT_HPP_

#include "servo_service.hpp"
#include "usart.hpp"

#define N_LEGS 		4
#define N_JOINTS	3

namespace robot {

	void init();

	void calibrate_servo(uint8_t leg, uint8_t joint);

	void move(void * arg);
	void min(void * arg);
	void max(void * arg);

	void loop_control( void );

}
;

#endif /* ROBOT_HPP_ */
