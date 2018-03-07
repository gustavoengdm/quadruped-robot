/*
 * serial_commander.hpp
 *
 *  Created on: Mar 7, 2018
 *      Author: gustavo
 */

#ifndef SCOMMAND_HPP_
#define SCOMMAND_HPP_

#include "new.hpp"
#include "usart.hpp"

// max length of a serial command
#define SERIAL_COMMANDER_BUFFER_SIZE		64

namespace scommand {

	template<uint8_t N> class SCommand {

		private:
			typedef struct {
					char * msg;
					void (*function)(void *);
			} Command;

			uint8_t comm_count;
			char endchar;

			volatile uint8_t new_command;
			uint8_t buff[ SERIAL_COMMANDER_BUFFER_SIZE];
			uint8_t buff_counter;
			uint8_t buff_start;

			void serial_rx_interrupt(void) {
				if (buff_counter >= SERIAL_COMMANDER_BUFFER_SIZE) {
					buff_counter = 0;
				}
				buff[buff_counter] = *(usart.__udr);
				if (buff[buff_counter] == '\r') {
					buff[buff_counter] = 0;
					return;
				}
				if (buff[buff_counter] == endchar) {
					buff[buff_counter] = 0;
					new_command = 1;
					buff_counter = 0;
					return;
				}
				buff_counter++;
			}

		public:
			Command commands[N];

			SCommand(uint16_t baud, char ch = '\n') :
					comm_count(0), endchar(ch), //
					new_command(0), buff_counter(0), buff_start(0) {

			}

			void add(const char * msg, void (*f)(void *)) {
				int sz;

				sz = strlen(msg);
				if (comm_count < N && sz < SERIAL_COMMANDER_BUFFER_SIZE) {
					commands[comm_count].msg = new char[sz];
					strcpy(commands[comm_count].msg, msg);
					commands[comm_count].function = f;
					comm_count++;
				}
			}
	};
}

#endif /* SCOMMAND_HPP_ */
