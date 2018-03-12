/*
 * main.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: gustavo
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "robot.hpp"

#include "usart.hpp"

//#define DEBUG_SCOMMAND
#include "scommand.hpp"
scommand::SCommand<4, 8> scom(usart);

// Used to create a static link between Scommand and usart's interrupt
void serial_rx_handler() {
	scom.serial_rx_interrupt();
}

void wrapper_min(void * s) {
	scommand::SCommand<4, 8> * scom = (scommand::SCommand<4, 8> *) s;
	int k = scom->get_next_integer();
	robot::max(&k);
}

void setup(void) {
	usart.begin_asynch(9600);
	usart.attach_rx_complete_interrupt(serial_rx_handler);

	scom.add("min", wrapper_min, 0x0);
	scom.add("min", robot::min, 0x0);
	scom.add("max", robot::max, 0x0);

	robot::init();

	sei();
}

void loop(void) {
	scom.proc();
	robot::loop_control();
}

int main() {
	setup();
	while (1)
		loop();
	return 0;
}

