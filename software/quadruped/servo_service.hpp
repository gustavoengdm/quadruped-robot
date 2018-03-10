/*
 * servo-service.hpp
 *
 *  Created on: Mar 4, 2018
 *      Author: gustavo
 */

#ifndef SERVO_SERVICE_HPP_
#define SERVO_SERVICE_HPP_

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

#include "com.hpp"

#define __prescaler				8
#define __clock_ticks_per_us	(F_CPU / 1000000L)
#define us2ticks( us )			( (__clock_ticks_per_us * (us)) / __prescaler )

#define MAX_NUMBER_OF_SERVOS 12

// definitions in microseconds
#define FRAME_PERIOD	20000		// 20ms
#define MIN_PULSE 		500			// 0.5ms
#define MAX_PULSE 		2000		// 2ms

// Using Timer1
namespace ServoService {

	uint8_t get_number_of_servos(void);

	class Servo {
		public:
			int8_t __index;
			uint16_t __min_ticks, __max_ticks;

		public:
			Servo();
			void attach(volatile uint8_t * port, uint8_t pin);
			void write_us(uint16_t us);
			void enable(uint8_t b);
	};

}

#endif /* SERVO_SERVICE_HPP_ */

