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
#include "usart.hpp"

ServoService::Servo s1;

void move(void) {
	_delay_ms(200);
	s1.write_us(500);

	_delay_ms(200);
	s1.write_us(2500);

	_delay_ms(200);
	s1.write_us(500);

	_delay_ms(500);
	s1.write_us(2500);

	_delay_ms(500);
	s1.write_us(1000);
}

volatile uint8_t new_command = 0;

#define SERIAL_BUFFER_SIZE		32
uint8_t buff[ SERIAL_BUFFER_SIZE] = { 0 };
uint8_t buffc = 0;
void serial_rx_int(void) {
	if (buffc >= SERIAL_BUFFER_SIZE) {
		buffc = 0;
	}
	buff[buffc] = *(usart.__udr);
	if (buff[buffc] == '\r') {
		buff[buffc] = 0;
		return;
	}
	if (buff[buffc] == '\n') {
		buff[buffc] = 0;
		new_command = 1;
		buffc = 0;
		return;
	}
	buffc++;
}

void setup(void) {

	usart.begin_asynch(9600, true, true, serial_rx_int);
	//usart.attach_rx_interrupt(serial_rx_int);

	s1.attach(&PORTB, 5);
	s1.write_us(1500);

	sei();
}

void loop(void) {

	if( new_command == 1 ) {
		new_command = 0;
		move();
	}

	_delay_ms(500);
	usart.print((uint8_t *) "I'm alive: ");
	usart.write('0' + buffc);
	usart.println((uint8_t *) buff);
}

int main() {
	setup();
	while (1)
		loop();
	return 0;
}

