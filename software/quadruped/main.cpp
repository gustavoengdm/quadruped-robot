/*
 * main.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: gustavo
 */

#include <avr/io.h>
#include "servo-service.hpp"

void setup(void) {
	ServoService::setup(20000);
}

void loop(void) {
}

int main() {

	setup();

	while(1) loop();

	return 0;
}

