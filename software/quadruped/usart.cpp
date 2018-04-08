/*
 * usart.cpp
 *
 *  Created on: Mar 8, 2018
 *      Author: gustavo
 */

#include "usart.hpp"

void USART::attach_rx_complete_interrupt(void (*rx_complete)(void)) {
	int_rx_complete = rx_complete;
	__sbi((*__ucsrb), RXCIE0);
}

void USART::attach_tx_complete_interrupt(void (*tx_complete)(void)) {
	int_tx_complete = tx_complete;
	__sbi((*__ucsrb), TXCIE0);
}

void USART::attach_udr_empty_interrupt(void (*udr_empty)(void)) {
	int_udr_empty = udr_empty;
	__sbi((*__ucsrb), UDRIE0);
}

uint8_t USART::read() {
	while (!((*__ucsra) & (1 << RXC0))) {
	};
	return UDR0;
}

void USART::write(uint8_t byte) {
	while (!((*__ucsra) & (1 << UDRE0))) {
	};
	UDR0 = byte;
}

void USART::write9(uint16_t byte) {
	// TODO: Add support to write 9bit character.
	// Write a nine bit on UCSR0B before UDR0
	UDR0 = byte;
}

void USART::print(const char * str) {
	int c = 0;
	while (str[c] != 0) {
		while (!((*__ucsra) & (1 << UDRE0)))
			;
		//UDR0 = str[c];
		(*__udr) = str[c];
		c++;
	}
}
void USART::println() {
	write('\n');
}
void USART::println(const char * str) {
	print(str);
	write('\n');
}

void USART::print(int n) {
	uint16_t k, i;
	if (n < 0) {
		write('-');
		n *= -1;
	}
	k = 0;
	i = 0;
	while( n ) {
		k = k*10 + n%10;
		n = n/10;
		i++;
	}
	while( i ) {
		n = k%10;
		write(n+'0');
		k = k / 10;
		i--;
	}
}

#ifdef HAVE_USART0
USART usart(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);
ISR( USART_RX_vect ) {
	if (usart.int_rx_complete)
	usart.int_rx_complete();
}
ISR( USART_TX_vect ) {
	if (usart.int_tx_complete)
	usart.int_tx_complete();
}
ISR( USART_UDRE_vect ) {
	if (usart.int_udr_empty)
	usart.int_udr_empty();
}
#endif

#ifdef HAVE_USART1
USART usart1(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);
ISR( USART_RX_vect ) {
	if (usart1.int_rx_complete)
	usart1.int_rx_complete();
}
ISR( USART_TX_vect ) {
	if (usart1.int_tx_complete)
	usart1.int_tx_complete();
}
ISR( USART_UDRE_vect ) {
	if (usart1.int_udr_empty)
	usart1.int_udr_empty();
}
#endif

