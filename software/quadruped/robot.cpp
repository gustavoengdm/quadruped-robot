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

		// tibia
		legs[0][0].attach(&PORTD, 2, 500, 2000);
		legs[1][0].attach(&PORTD, 5, 500, 2000);
		legs[2][0].attach(&PORTB, 0, 500, 2000);
		legs[3][0].attach(&PORTB, 3, 500, 2000);

		// femur
		legs[0][1].attach(&PORTD, 3, 500, 2000);
		legs[1][1].attach(&PORTD, 6, 500, 2000);
		legs[2][1].attach(&PORTB, 1, 500, 2000);
		legs[3][1].attach(&PORTB, 4, 500, 2000);

		// coxa
		legs[0][2].attach(&PORTD, 4, 500, 2000);
		legs[1][2].attach(&PORTD, 7, 500, 2200);
		legs[2][2].attach(&PORTB, 2, 500, 2000);
		legs[3][2].attach(&PORTB, 5, 500, 2000);

		usart.print("Number of servos: ");
		usart.write(ServoService::get_number_of_servos() + '0');
		usart.print("\n");

		for (l = 0; l < N_LEGS; l++) {
			for (j = 0; j < N_JOINTS; j++) {
				//legs[l][j].write_us(MIN_PULSE);
				legs[l][j].write_us(800);
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

	void set_joint(uint8_t joint, uint16_t ticks) {
		int l;
		for (l = 0; l < N_LEGS; l++) {
			legs[l][joint].write_us(ticks);
		}
	}

	void set_angle(uint8_t leg, uint8_t joint, uint16_t angle) {
		legs[leg][joint].set_angle((float) angle);
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
