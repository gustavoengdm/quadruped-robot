/*
 * servo_service.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: gustavo
 */

#include "servo_service.hpp"
#include "usart.hpp"

namespace ServoService {

	class __servo_pin {
		public:
			uint8_t pin;
			volatile uint8_t * port;
			volatile uint16_t ticks;
			volatile uint16_t min_ticks, max_ticks;
			volatile uint8_t active;

		public:
			inline void low(void) {
				__cbi(*port, pin);
			}

			inline void high(void) {
				__sbi(*port, pin);
			}
	};

	//volatile uint16_t __frame_ticks;
	volatile static uint8_t __nbr_of_servos = 0;
	volatile static int8_t __servo_count = -1;

	uint8_t get_number_of_servos(void) {
		return __nbr_of_servos;
	}

	__servo_pin servos[MAX_NUMBER_OF_SERVOS];

	/**
	 * Configure Timer1 to ServoService
	 * @param us frame period in microseconds
	 */
	//void setup(uint16_t us) {
	static void init(void) {

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

	Servo::Servo() {
		__index = 0;
	}

	//void Servo::attach(volatile uint8_t * port, uint8_t pin, uint16_t ticks) {
	void Servo::attach(volatile uint8_t * port, uint8_t pin, uint16_t ticks_min, uint16_t ticks_max) {

		if (__nbr_of_servos == 0) {
			init();
		}

		if (__nbr_of_servos >= MAX_NUMBER_OF_SERVOS) {
			__index = -1;
			return;
		}

		__index = __nbr_of_servos;
		servos[__index].active = false;
		servos[__index].port = port;
		servos[__index].pin = pin;
		servos[__index].ticks = us2ticks(MIN_PULSE);
		servos[__index].min_ticks = ticks_min;
		servos[__index].max_ticks = ticks_max;

		// Accessing DDRx to configure pin as OUTPUT
		*(port - 1) |= (1 << pin);

		// Cleaning pin level
		*port &= ~(1 << pin);

		// increase number of attached servos
		__nbr_of_servos++;
	}

	void Servo::write_us(uint16_t us) {
		servos[__index].active = true;
		servos[__index].ticks = us2ticks(us);
	}

	void Servo::set_angle( float a ) {
		int16_t us, maxt, mint;

		if( a < 0 ) a = 360 + a;
		if( a != 0) a = a / 180.0;

		maxt = servos[__index].max_ticks;
		mint = servos[__index].min_ticks;

		usart.print("max_ticks: ");
		usart.print( (int)(maxt) );
		usart.println();
		usart.print("min_ticks: ");
		usart.print( (int)(mint) );
		usart.println();

		usart.print("diff: ");
		usart.print( (int)(maxt-mint) );
		usart.println();

		us = (maxt-mint) * a + mint;

		usart.print("diff(us): ");
		usart.print( (int)(us) );
		usart.println();
		write_us( us );
	}

	void Servo::enable(uint8_t b) {
		servos[__index].active = b;
	}

	void Servo::min(void) {
		write_us(servos[__index].min_ticks);
	}
	void Servo::max(void) {
		write_us(servos[__index].max_ticks);
	}

	void Servo::define_min(uint16_t ticks) {
		servos[__index].min_ticks = ticks;
	}
	void Servo::define_max(uint16_t ticks) {
		servos[__index].max_ticks = ticks;
	}

}

// action
ISR(TIMER1_COMPA_vect) {
	ServoService::handle_interrupt();
}

