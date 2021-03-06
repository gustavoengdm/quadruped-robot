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
scommand::SCommand<6, 12> scom(usart);

// Used to create a static link between Scommand and usart's interrupt
void serial_rx_handler() {
	scom.serial_rx_interrupt();
}

void wrapper_set(void * s) {
	int ticks;

	scommand::SCommand<4, 12> * scom = (scommand::SCommand<4, 12> *) s;
	ticks = scom->get_next_integer();
	robot::set_all(ticks);
}

void wrapper_setj(void * s) {
	int ticks, joint;

	scommand::SCommand<4, 12> * scom = (scommand::SCommand<4, 12> *) s;
	joint = scom->get_next_integer();
	ticks = scom->get_next_integer();
	robot::set_joint(joint, ticks);
}

void wrapper_move(void * s) {
	int leg, joint, ticks;

	scommand::SCommand<4, 12> * scom = (scommand::SCommand<4, 12> *) s;
	leg = scom->get_next_integer();
	joint = scom->get_next_integer();
	ticks = scom->get_next_integer();
	robot::move_leg(leg, joint, ticks);
}

void help(void * s) {
	usart.println("--------------------------");
	usart.println("help");
	usart.println("set <us>");
	usart.println("sj <joint> <us>");
	usart.println("mvl <leg> <joint> <us>");
	usart.println("sa <leg> <joint> <angle>");
	usart.println("min");
	usart.println("max");
	usart.println("--------------------------");
}

void wrapper_set_angle(void * s) {
	int angle, joint, leg;

	scommand::SCommand<4, 12> * scom = (scommand::SCommand<4, 12> *) s;
	leg = scom->get_next_integer();
	joint = scom->get_next_integer();
	angle = scom->get_next_integer();
	robot::set_angle(leg, joint, angle);
}

void setup(void) {
	usart.begin_asynch(9600);
	usart.attach_rx_complete_interrupt(serial_rx_handler);

	scom.add("help", help, 0x0);			// help
	scom.add("set", wrapper_set, &scom);	// set <us>
	scom.add("sj", wrapper_setj, &scom);	// sj <joint> <us>
	scom.add("mvl", wrapper_move, &scom);	// mvl <leg> <joint> <us>
	scom.add("sa", wrapper_set_angle, &scom);	// sa <leg> <joint> <angle>
	scom.add("min", robot::min, 0x0);		// min
	scom.add("max", robot::max, 0x0);		// max

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
	usart.print(-12000);
	while (1)
		loop();
	return 0;
}

