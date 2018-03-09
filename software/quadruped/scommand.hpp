/*
 * serial_commander.hpp
 *
 *  Created on: Mar 7, 2018
 *      Author: gustavo
 */

#ifndef SCOMMAND_HPP_
#define SCOMMAND_HPP_

//#include "new.hpp"
#include <stdlib.h>
#include "usart.hpp"

namespace scommand {

	template<int8_t N_COMMANDS, uint8_t COMMAND_MAX_SIZE, uint8_t COMM_BUFFER_SIZE = 32>
	class SCommand {

		public:
			USART * __usart;				// USART to deal

			char __buff[COMM_BUFFER_SIZE];	// buffer message
			uint8_t buff_pos;

			char endchar;					// character command terminator
			char delim;						// string separator

			typedef struct {
					char event_name[COMMAND_MAX_SIZE];
					void (*function)(void);
			} CommandCall;
			CommandCall command_list[N_COMMANDS];						// vector of commands
			uint8_t comm_count;				// number of commands inserted

			volatile uint8_t new_comm;		// flag to indicates a new command

		public:

			SCommand(USART &us, char ch = '\n', char delim = ' ') :
					__usart(&us), //
					buff_pos(0), //
					endchar(ch), delim(delim), //
					comm_count(0), //
					new_comm(0) {
			}

			volatile const char * const get_buffer(void) const {
				return __buff;
			}

			void add(const char * msg, void (*f)(void)) {
				int sz;
				if (comm_count < N_COMMANDS) {
					sz = strlen(msg);
					if (sz < COMMAND_MAX_SIZE - 1) {
						strncpy(command_list[comm_count].event_name, msg, sz);
						command_list[comm_count].function = f;
						comm_count++;
					}
#ifdef DEBUG_SCOMMAND
					else {
						__usart->print("The command name (");
						__usart->print( msg );
						__usart->println(") is too large.");
						return;
					}
#endif
				}
#ifdef DEBUG_SCOMMAND
				else {
					__usart->println("The command list is full!");
					return;
				}
#endif
			}

#ifdef DEBUG_SCOMMAND
			void show_commands(void) {
				int c;
				for (c = 0; c < comm_count; c++) {
					__usart->print("Command (");
					__usart->write('0' + c);
					__usart->print("): ");
					__usart->println(command_list[c].event_name);
				}
			}
#endif

			void serial_rx_interrupt(void) {
				if (buff_pos >= COMM_BUFFER_SIZE) {
					buff_pos = 0;
				}
				__buff[buff_pos] = *(usart.__udr);
				if (__buff[buff_pos] == '\r') {
					__buff[buff_pos] = 0;
					return;
				}
				if (__buff[buff_pos] == endchar) {
					__buff[buff_pos] = 0;
					new_comm = 1;
					buff_pos = 0;
#ifdef DEBUG_SCOMMAND
					__usart->println("serial_rx_interrupt: end of command");
#endif
					return;
				}
				buff_pos++;
			}

			int8_t command_match(const char * str) {
				uint8_t c;
				size_t sz = strlen(str);
				for (c = 0; c < comm_count; c++) {
					if (strncmp(str, command_list[c].event_name, sz) == 0) {
						return c;
					}
				}
				return -1;
			}

			void proc() {
				int8_t c;
				if (!new_comm) return;

				c = command_match(__buff);
				if (c > -1) {
					command_list[c].function();
				}
				new_comm = 0;
			}
	};

}

#endif /* SCOMMAND_HPP_ */
