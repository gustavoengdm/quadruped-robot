/*
 * robot.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: gustavo
 */

#include <avr/io.h>
#include <math.h>

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
		legs[1][0].attach(&PORTD, 5, MAX_PULSE, MIN_PULSE);
		legs[1][1].attach(&PORTD, 6, MAX_PULSE, MIN_PULSE);
		legs[1][2].attach(&PORTD, 7, MAX_PULSE, MIN_PULSE);
		legs[2][0].attach(&PORTB, 0, MAX_PULSE, MIN_PULSE);
		legs[2][1].attach(&PORTB, 1, MAX_PULSE, MIN_PULSE);
		legs[2][2].attach(&PORTB, 2, MAX_PULSE, MIN_PULSE);
		legs[3][0].attach(&PORTB, 3);
		legs[3][1].attach(&PORTB, 4);
		legs[3][2].attach(&PORTB, 5);

		usart.print("Number of servos: ");
		usart.write(ServoService::get_number_of_servos() + '0');
		usart.print("\n");

		for (l = 0; l < N_LEGS; l++) {
			for (j = 0; j < N_JOINTS; j++) {
				legs[l][j].write_us(MIN_PULSE);
			}
		}
	}

	void calibrate_servo(uint8_t leg, void * arg) {
		//legs[0]][0].
	}

	float rect2polar(float x, float y, float z) {
		float lc, lf, lt;
		float zo = 1;
		float tc, tf, tt;
		float a1, a2;
		float L;

		tc = atan2(y, x);

		L = sqrt(zo * zo + (x - lc) * (x - lc));

		a1 = acos(zo / sqrt(L));
		a2 = acos((lf * lf + L * L - lt * lt) / (2 * lf * L));

		tf = a1 + a2;

		tt = 1;

		return tc + tf + tt;
	}

	void min(void * arg) {
		int l, j;
		for (l = 0; l < N_LEGS; l++) {
			for (j = 0; j < N_JOINTS; j++) {
				//legs[l][j].write_us(500);
				legs[l][j].min();
			}
		}
	}

	void max(void * arg) {
		int l, j;
		for (l = 0; l < N_LEGS; l++) {
			for (j = 0; j < N_JOINTS; j++) {
				//legs[l][j].write_us( 500 );
				legs[l][j].max();
			}
		}
	}

	void set_all(uint16_t ticks) {
		int l, j;
		for (l = 0; l < N_LEGS; l++) {
			for (j = 0; j < N_JOINTS; j++) {
				legs[l][j].write_us(ticks);
			}
		}
	}

	void loop_control(void) {

	}

	void move_leg(uint8_t leg, uint8_t joint, uint16_t ticks) {
		legs[leg][joint].write_us(ticks);
	}

}
