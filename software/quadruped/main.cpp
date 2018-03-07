/*
 * main.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: gustavo
 */

/**
 avrdude -Cavrdude.conf -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:/mnt/wdisk/work/projects/robotics/quadruped-robot/eclipse-workspace/tmp/Release/tmp.hex:i
 **/

#define DEBUG	1


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "servo_service.hpp"
#include "usart.hpp"
#include "scommand.hpp"

scommand::SCommand<1> scomm(9600, '\n');

ServoService::Servo s1;

void move(void) {

	_delay_ms(200);
	s1.write_us(500);
	//s2.write_us(1500);

	_delay_ms(200);
	s1.write_us(2500);
	//s2.write_us(500);

	_delay_ms(200);
	s1.write_us(500);
	//s2.write_us(1000);

	_delay_ms(500);
	s1.write_us(2500);
	//s2.write_us(500);

	_delay_ms(500);
	s1.write_us(1000);
	//s2.write_us(2500);
}

void setup(void) {
	scomm.add("move", 0x0);

	s1.attach(&PORTB, 2);
	s1.write_us(1500);
	sei();
}

void loop(void) {
	_delay_ms(500);
	usart.print("I'm alive\n");
}

int main() {
	setup();
	while (1)
		loop();
	return 0;
}

