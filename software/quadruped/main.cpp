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
//ServoService::Servo s2;
//ServoService::Servo s3;

void setup(void) {

	s1.attach(&PORTB, 5);
	s1.write_us(2500);

//	s2.attach(&PORTB, 5);
//	s2.write_us(2500);
//	s2.enable(0);
//
//	s3.attach(&PORTB, 5);
//	s3.write_us(5000);

	sei();
}

void loop(void) {
}

int main() {
	setup();
	while (1) loop();
	return 0;
}

