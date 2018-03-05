/*
 * main.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: gustavo
 */

/**
 avrdude -Cavrdude.conf -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:/mnt/wdisk/work/projects/robotics/quadruped-robot/eclipse-workspace/tmp/Release/tmp.hex:i
 **/

#include <avr/io.h>
#include <util/delay.h>

#include "servo_service.hpp"

ServoService::Servo s1;
ServoService::Servo s2;

void setup(void) {

	ServoService::setup(20000);
	DDRB = 0xFF;
	s1.attach(&PORTB, 5);
	s1.write_us(15000);
	s2.attach(&PORTB, 4);
	s2.write_us(10000);

	sei();
}

// action
//ISR (TIMER1_COMPA_vect) {
//	//_delay_ms(50);
//	PORTB ^= (1 << 5);
//}

void loop(void) {
//	_delay_ms(50);
//	PORTB ^= (1 << 5);
}

int main() {

	setup();

	while (1)
		loop();

	return 0;
}

