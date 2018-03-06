/*
 * servo-service.hpp
 *
 *  Created on: Mar 4, 2018
 *      Author: gustavo
 */

#ifndef SERVO_SERVICE_HPP_
#define SERVO_SERVICE_HPP_

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

#include "com.hpp"

#define __prescaler				8
#define __clock_ticks_per_us	(F_CPU / 1000000L)
#define us2ticks( us )			( (__clock_ticks_per_us * (us)) / __prescaler )

#define MAX_NUMBER_OF_SERVOS 12

// definitions in microseconds
#define FRAME_PERIOD	20000		// 20ms
#define MIN_PULSE 		500			// 0.5ms
#define MAX_PULSE 		2000		// 2ms

// Using Timer1
namespace ServoService {

	class __servo {
		public:
			uint8_t pin;
			volatile uint8_t * port;
			volatile uint16_t ticks;
			volatile uint8_t active;

		public:
			inline void low(void) {
				__cbi(*port, pin);
			}

			inline void high(void) {
				__sbi(*port, pin);
			}
	};

	__servo servos[MAX_NUMBER_OF_SERVOS];

	//volatile uint16_t __frame_ticks;
	volatile uint8_t __nbr_of_servos = 0;
	volatile int8_t __servo_count = -1;

	/**
	 * Configure Timer1 to ServoService
	 * @param us frame period in microseconds
	 */
	//void setup(uint16_t us) {
	void init(void) {

		// compute the number of ticks for the frame period
		//__frame_ticks = us2ticks(us);
		__nbr_of_servos = 0;
		__servo_count = -1;

		// set Normal Mode operation
		TCCR1A = 0;

		// configure prescaler 8
		TCCR1B = (1 << CS11);

		// start timer1
		TCNT1 = 0;

		// clear OCF1A to prevent any old interrupts
		TIFR1 |= (1 << OCF1A);

		// enable interrupt of Output Compare Register A
		TIMSK1 |= (1 << OCIE1A);

	}

	class Servo {
		public:
			int8_t __index;
			uint16_t __min_ticks, __max_ticks;

		public:
			Servo() {
				__index = 0;
				__min_ticks = us2ticks(MIN_PULSE);
				__max_ticks = us2ticks(MAX_PULSE);
			}

			void attach(volatile uint8_t * port, uint8_t pin) {

				if (__nbr_of_servos == 0) {
					init();
				}

				if (__nbr_of_servos >= MAX_NUMBER_OF_SERVOS) {
					__index = -1;
					return;
				}

				__index = __nbr_of_servos;
				servos[__index].active = true;
				servos[__index].port = port;
				servos[__index].pin = pin;
				servos[__index].ticks = __min_ticks;

				// Accessing DDRx to configure pin as OUTPUT
				*(port - 1) |= (1 << pin);

				// Cleaning pin level
				*port &= ~(1 << pin);

				// increase number of attached servos
				__nbr_of_servos++;
			}

			inline void write_us(uint16_t us) {
				servos[__index].ticks = us2ticks(us);
			}

			inline void enable(uint8_t b = 1) {
				servos[__index].active = b;
			}
	};

	static void handle_interrupt(void) {

		if (__servo_count == -1) {
			// restart counter
			TCNT1 = 0;
		} else {
			// set current Servo pin to low
			if (__servo_count < __nbr_of_servos && servos[__servo_count].active) {
				servos[__servo_count].low();
			}
		}

		__servo_count++;

		if (__servo_count < __nbr_of_servos) {

			// update OCR1A to the next TOP value
			OCR1A = TCNT1 + servos[__servo_count].ticks;

			// Set servo pin high if it is active
			if (servos[__servo_count].active) {
				servos[__servo_count].high();
			}

		} else { // if (__servo_count == __nbr_of_servos) {

			//if ((TCNT1 + 4) < __frame_ticks) {
			if ((TCNT1 + 4) < us2ticks(FRAME_PERIOD)) {
				//OCR1A = __frame_ticks;
				OCR1A = us2ticks(FRAME_PERIOD);
			} else {
				OCR1A = TCNT1 + 4;
			}
			__servo_count = -1;
		}

	}

}

// action
ISR(TIMER1_COMPA_vect) {
	ServoService::handle_interrupt();
}

#endif /* SERVO_SERVICE_HPP_ */

