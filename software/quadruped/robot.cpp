/*
 * robot.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: gustavo
 */

#include <avr/io.h>

#include "usart.hpp"
#include "robot.hpp"

#include "scommand.hpp"

namespace robot {

	ServoService::Servo legs[ N_LEGS][ N_JOINTS];

	void init(void) {
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
		usart.write(ServoService::get_number_of_servos() + '0');
		usart.print("\n");

		for (l = 0; l < N_LEGS; l++) {
			for (j = 0; j < N_JOINTS; j++) {
				legs[l][j].write_us(1000);
			}
		}
	}

	void calibrate_servo(uint8_t leg, void * arg) {
		//legs[0]][0].
	}

	void min(void * arg) {
		legs[0][0].write_us(600);
	}

	void max(void * arg) {
		legs[0][0].write_us(2000);
	}

	void loop_control(void) {

	}

}
