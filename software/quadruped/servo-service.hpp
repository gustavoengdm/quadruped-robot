/*
 * servo-service.hpp
 *
 *  Created on: Mar 4, 2018
 *      Author: gustavo
 */

#ifndef SERVO_SERVICE_HPP_
#define SERVO_SERVICE_HPP_

#include <avr/io.h>
#include <stdint.h>


#define __prescaler			8
#define __clock_period		( (1.0 * __prescaler) / F_CPU )
#define us2ticks( us )		( (us / 1000000) / __clock_period )

// Using Timer1
namespace ServoService {


	void setup( uint16_t us ) {
		OCR1A = us2ticks( us );
	}

	class Servo {
		public:
			Servo( uint16_t min );
	};

}

#endif /* SERVO_SERVICE_HPP_ */
