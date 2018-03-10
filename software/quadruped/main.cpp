/*
 * main.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: gustavo
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

//#include "servo_service.hpp"
//ServoService::Servo s1;
//ServoService::Servo s2;

#include "robot.hpp"

#include "usart.hpp"

#define DEBUG_SCOMMAND
#include "scommand.hpp"
scommand::SCommand<4, 15> scom(usart);

// Used to create a static link between Scommand and usart's interrupt
inline void serial_rx_handler() {
	scom.serial_rx_interrupt();
}

void setup(void) {
	usart.begin_asynch(9600);
	usart.attach_rx_complete_interrupt(serial_rx_handler);

	robot::init();

	scom.add("min", robot::min);
	scom.add("max", robot::max);

	sei();
}

void loop(void) {
	scom.proc();
}

int main() {
	setup();
	while (1)
		loop();
	return 0;
}

