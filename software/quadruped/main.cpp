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
#include <string.h>

#include "servo_service.hpp"
ServoService::Servo s1;

#include "usart.hpp"
USE_USART

#include "scommand.hpp"
scommand::SCommand<3, 12> scom(usart);

// Used to create a static interrupt link between Scommand and usart
inline void serial_rx_handler() {
	scom.serial_rx_interrupt();
}


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
	usart.begin_asynch(9600);
	usart.attach_rx_interrupt( serial_rx_handler );

	scom.add( "move", move );

	s1.attach(&PORTB, 5);
	s1.write_us(1000);
	sei();
}

void loop(void) {
	scom.proc();
	_delay_ms(1500);
	usart.print("I'm alive ... | ");
	usart.println( scom.buff );
	//move();
}

int main() {
	setup();
	while (1)
		loop();
	return 0;
}

