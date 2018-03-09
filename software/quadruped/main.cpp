/*
 * main.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: gustavo
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "servo_service.hpp"
ServoService::Servo s1;

#include "usart.hpp"

#define DEBUG_SCOMMAND
#include "scommand.hpp"
scommand::SCommand<3, 10> scom(usart);

// Used to create a static link between Scommand and usart's interrupt
inline void serial_rx_handler() {
	scom.serial_rx_interrupt();
}

void max(void) {

	_delay_ms(200);
	s1.write_us(2500);
}

void min(void) {

	_delay_ms(200);
	s1.write_us(500);
}

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

void setup(void) {
	usart.begin_asynch(9600);
	usart.attach_rx_complete_interrupt(serial_rx_handler);

	scom.add("move", move);
	scom.add("max", max);
	scom.add("min", min);

	s1.attach(&PORTB, 3);
	s1.write_us(1000);
	sei();
}

void loop(void) {
	scom.proc();
	//_delay_ms(500);
	//scom.show_commands();
	//usart.println( scom.get_buffer() );
	//move();
}

int main() {
	setup();
	while (1)
		loop();
	return 0;
}

