/*
 * robot.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: gustavo
 */

#include <avr/io.h>

#include "usart.hpp"
#include "robot.hpp"

namespace robot {

	ServoService::Servo legs[ N_LEGS][ N_JOINTS];

	void init() {
		uint8_t l, j;

		legs[0][0].attach(&PORTD, 2);
		legs[0][1].attach(&PORTD, 3);
		legs[0][2].attach(&PORTD, 4);
		legs[1][0].attach(&PORTD, 5);
		legs[1][1].attach(&PORTD, 6);
		legs[1][2].attach(&PORTD, 7);
		legs[2][0].attach(&PORTB, 0);
		legs[2][1].attach(&PORTB, 1);
		legs[2][2].attach(&PORTB, 2);
		legs[3][0].attach(&PORTB, 3);
		legs[3][1].attach(&PORTB, 4);
		legs[3][2].attach(&PORTB, 5);

		usart.print("Number of servos: ");
		usart.write( ServoService::get_number_of_servos() + '0' );
		usart.print("\n");

		for (l = 0; l < 1/*N_LEGS*/; l++) {
			for (j = 0; j < N_JOINTS; j++) {
				legs[l][j].write_us(600);
			}
		}
	}

	void min(void) {
		legs[0][0].write_us(600);
	}

	void max(void) {
			legs[0][0].write_us(2000);
		}

}
