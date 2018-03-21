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
scommand::SCommand<4, 12> scom(usart);

// Used to create a static link between Scommand and usart's interrupt
void serial_rx_handler() {
	scom.serial_rx_interrupt();
}

void wrapper_set(void * s) {
	scommand::SCommand<4, 12> * scom = (scommand::SCommand<4, 12> *) s;
	int ticks;

	ticks = scom->get_next_integer();

	robot::set_all(ticks);

}

void wrapper_move(void * s) {
	scommand::SCommand<4, 12> * scom = (scommand::SCommand<4, 12> *) s;
	int leg, joint, ticks;

	leg = scom->get_next_integer();
	joint = scom->get_next_integer();
	ticks = scom->get_next_integer();

	robot::move_leg(leg, joint, ticks);

}

void setup(void) {
	usart.begin_asynch(9600);
	usart.attach_rx_complete_interrupt(serial_rx_handler);

	scom.add("set", wrapper_set, &scom);
	scom.add("mvl", wrapper_move, &scom);
	scom.add("min", robot::min, 0x0);
	scom.add("max", robot::max, 0x0);

	robot::init();

	sei();
}

void loop(void) {
	scom.proc();
	robot::loop_control();
	//float x = robot::rect2polar(1,1,1);
}

int main() {
	setup();
	while (1)
		loop();
	return 0;
}

