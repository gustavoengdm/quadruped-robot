/*
 * USART.hpp
 *
 *  Created on: Feb 7, 2018
 *      Author: gustavo
 */

#ifndef USART_HPP_
#define USART_HPP_

#include "com.hpp"

#include <avr/io.h>
#include <avr/interrupt.h>

class USART {

	private:
		volatile uint8_t * const __ubrrh;
		volatile uint8_t * const __ubrrl;
		volatile uint8_t * const __ucsra;
		volatile uint8_t * const __ucsrb;
		volatile uint8_t * const __ucsrc;
		volatile uint8_t * const __udr;

	public:
		static bool (*int_rx_complete)(void);
		static bool (*int_tx_complete)(void);
		static bool (*int_udr_empty)(void);

		typedef enum {
			ASHYNC = 0,
			SYNC_MASTER = 1,
			SPI_MASTER = 3
		} operation_mode;

		typedef enum {
			PARITY_DISABLED = 0b00,
			PARITY_EVEN = 0b10,
			PARITY_ODD = 0b11
		} parity_mode;

		typedef enum {
			CHAR_SIZE_5 = 0b000,
			CHAR_SIZE_6 = 0b001,
			CHAR_SIZE_7 = 0b010,
			CHAR_SIZE_8 = 0b011,
			CHAR_SIZE_9 = 0b111,
		} character_size;

		typedef enum {
			STOP_BIT_1 = 1,
			STOP_BIT_2 = 2
		} stop_bit_size;

		USART(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl, volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
				volatile uint8_t *ucsrc, volatile uint8_t *udr) :
				__ubrrh(ubrrh), __ubrrl(ubrrl), __ucsra(ucsra), __ucsrb(ucsrb), __ucsrc(ucsrc), __udr(udr) {
		}

		/**
		 * TODO: Add Data Register Empty Interrupt and Error Flags
		 */
		void begin_asynch(
				unsigned long int baud = 9600, //
				bool tx_enable = true, //
				bool rx_enable = true, //
				bool (*rx_complete)(void) = 0x0, //
				bool (*tx_complete)(void) = 0x0, //
				bool (*udr_empty)(void) = 0x0, //
				character_size char_size = CHAR_SIZE_8, //
				parity_mode parity = PARITY_DISABLED, //
				stop_bit_size stopbit = STOP_BIT_1 //
				) {
			// Setting operation mode to Asynchronous Mode. UCSR0C [00-- ----]
			// UCSR0C &= ~((1 << UMSEL01) | (1 << UMSEL00));
			__cbi((*__ucsrc), UMSEL00);
			__cbi((*__ucsrc), UMSEL01);

			// Setting baud rate
			uint16_t u1 = (F_CPU / (16 * baud) - 1) + 0.5;
			uint16_t u2 = (F_CPU / (8 * baud) - 1) + 0.5;

			float e1 = ((float) u1) / baud - 1;
			float e2 = ((float) u2) / baud - 1;

			if ( __abs(e1) > __abs(e2)) {
				__sbi((*__ucsra), U2X0);
				u1 = u2;
			} else {
				__cbi((*__ucsra), U2X0);
			}
			(*__ubrrl) = (u1 & 0xff);
			(*__ubrrh) = (u1 >> 8);

			// Setting parity mode to parity. UCSR0C [--xx ----]
			switch (parity) {
				case PARITY_DISABLED:
					__cbi((*__ucsrc), UPM00);
					__cbi((*__ucsrc), UPM01);
					break;
				case PARITY_EVEN:
					__cbi((*__ucsrc), UPM00);
					__sbi((*__ucsrc), UPM01);
					break;
				case PARITY_ODD:
					__sbi((*__ucsrc), UPM00);
					__sbi((*__ucsrc), UPM01);
					break;
			}

			// Setting stop bit size. UCSR0C [---- x---]
			switch (stopbit) {
				case STOP_BIT_1:
					// (*_ucsrc) &= ~(1 << USBS0);
					__cbi((*__ucsrc), USBS0);
					break;
				case STOP_BIT_2:
					// UCSR0C |= (1 << USBS0);
					__sbi((*__ucsrc), USBS0);
					break;
			}

			// Setting character size. UCSR0B [---- -x--], UCSR0C [---- -xx-]
			switch (char_size) {
				case CHAR_SIZE_5:
					__cbi((*__ucsrb), UCSZ02);
					__cbi((*__ucsrc), UCSZ01);
					__cbi((*__ucsrc), UCSZ00);
					break;
				case CHAR_SIZE_6:
					__cbi((*__ucsrb), UCSZ02);
					__cbi((*__ucsrc), UCSZ01);
					__sbi((*__ucsrc), UCSZ00);
					break;
				case CHAR_SIZE_7:
					__cbi((*__ucsrb), UCSZ02);
					__sbi((*__ucsrc), UCSZ01);
					__cbi((*__ucsrc), UCSZ00);
					break;
				case CHAR_SIZE_8:
					// Setting character size
					// UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
					// (*_ucsrb) &= ~(1 << UCSZ02);
					__cbi((*__ucsrb), UCSZ02);
					__sbi((*__ucsrc), UCSZ01);
					__sbi((*__ucsrc), UCSZ00);
					break;
				case CHAR_SIZE_9:
					__sbi((*__ucsrb), UCSZ02);
					__sbi((*__ucsrc), UCSZ01);
					__sbi((*__ucsrc), UCSZ00);
					break;
			}

			// Enabling interrupts. UCSR0B [xx-- ----]
			// Enable interrupts
			// UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0);
			if (rx_complete) __sbi((*__ucsrb), RXCIE0);
			else __cbi((*__ucsrb), RXCIE0);
			USART::int_rx_complete = rx_complete;

			if (tx_complete) __sbi((*__ucsrb), TXCIE0);
			else __cbi((*__ucsrb), TXCIE0);
			USART::int_tx_complete = tx_complete;

			if (udr_empty) __sbi((*__ucsrb), UDRIE0);
			else __cbi((*__ucsrb), UDRIE0);
			USART::int_udr_empty = udr_empty;

			// Enabling RX e TX
			// Enable RX and TX
			// UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
			if (rx_enable) __sbi((*__ucsrb), RXEN0);
			else __cbi((*__ucsrb), RXEN0);

			if (tx_enable) __sbi((*__ucsrb), TXEN0);
			else __cbi((*__ucsrb), TXEN0);
		}

		uint8_t read() {
			while (!((*__ucsra) & (1 << RXC0)))
				;
			return UDR0;
		}

		void write(uint8_t byte) {
			while (!((*__ucsra) & (1 << UDRE0)))
				;

			UDR0 = byte;
		}

		void write9(uint16_t byte) {
			// TODO: Add support to write 9bit character.
			// Write a nine bit on UCSR0B before UDR0
			UDR0 = byte;
		}

		void print(const uint8_t * str) {

			int c = 0;
			while (str[c] != 0) {
				while (!((*__ucsra) & (1 << UDRE0)))
					;
				//UDR0 = str[c];
				(*__udr) = str[c];
				++c;
			}

		}
		void println() {
			write('\n');
		}
		void println(const uint8_t * str) {
			print(str);
			write('\n');
		}

};

/* Static attributes initialization */
bool (*USART::int_rx_complete)(void) = 0;
bool (*USART::int_tx_complete)(void) = 0;
bool (*USART::int_udr_empty)(void) = 0;

ISR( USART_RX_vect ) {
	if (USART::int_rx_complete) USART::int_rx_complete();
}

ISR( USART_TX_vect ) {
	if (USART::int_tx_complete) USART::int_tx_complete();
}

ISR( USART_UDRE_vect ) {
	if (USART::int_udr_empty) USART::int_udr_empty();
}

USART usart(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);

#endif /* USART_HPP_ */
