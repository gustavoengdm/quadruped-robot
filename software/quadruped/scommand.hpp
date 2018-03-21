/*
 * serial_commander.hpp
 *
 *  Created on: Mar 7, 2018
 *      Author: gustavo
 */

#ifndef SCOMMAND_HPP_
#define SCOMMAND_HPP_

//#include "new.hpp"
//#include <stdlib.h>
#include <string.h>
#include "usart.hpp"

namespace scommand {

//	void string_cpyn( char * dest, const char * orig, uint16_t n ) {
//
//	}

	template<int8_t N_COMMANDS, uint8_t COMMAND_MAX_SIZE, uint8_t COMM_BUFFER_SIZE = 32>
	class SCommand {

		public:
			USART * __usart;						// USART to deal

			char __buff[COMM_BUFFER_SIZE + 1];		// buffer message
			uint8_t buff_pos;						// counter for buffer fill

			char endchar;							// character command terminator
			char delim;								// string separator

			uint8_t arg_pos;						// counter to deal with arguments

			typedef struct {
					char event_name[COMMAND_MAX_SIZE + 1];
					uint8_t sz;						// size of event name
					void (*function)(void *);
					void * arg;
			} CommandCall;
			CommandCall command_list[N_COMMANDS];	// vector of commands
			uint8_t comm_count;						// number of inserted commands

			volatile uint8_t new_comm;				// flag to indicates new command was reached

		public:

			SCommand(USART &us, char ch = '\n', char delim = ' ') :
					__usart(&us), //
					buff_pos(0), //
					endchar(ch), delim(delim), //
					arg_pos(0), //
					comm_count(0), //
					new_comm(0) {
			}

			volatile const char * const get_buffer(void) const {
				return __buff;
			}

			void add(const char * msg, void (*f)(void *), void * arg) {
				int sz;
				if (comm_count < N_COMMANDS) {
					sz = strlen(msg);
					if (sz < COMMAND_MAX_SIZE - 1) {
						strncpy(command_list[comm_count].event_name, msg, sz);
						command_list[comm_count].function = f;
						command_list[comm_count].arg = arg;
						command_list[comm_count].sz = sz;
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
					return;
				}
				buff_pos++;
			}

			/**
			 * Match the input string with command list. If the match is true, returns the index of command callback.
			 * @param str input string
			 * @return the index of the matched command callback or -1 if the string is not matched.
			 */
			int8_t match_command(const char * str) {
				uint8_t c;
				size_t sz;
				arg_pos = 0;

				for (c = 0; c < comm_count; c++) {
					//sz = strlen(command_list[c].event_name);
					sz = command_list[c].sz;
					if (strncmp(str, command_list[c].event_name, sz) == 0) {

						if (str[sz] == '\0') return c;
						if (str[sz] == delim) {
							arg_pos = sz;
							return c;
						}
					}
				}

				return -1;
			}

			void split_comm(void) {

			}

			void proc() {
				int8_t comm_idx;
				if (!new_comm) return;

				comm_idx = match_command(__buff);
				if (comm_idx > -1) {
#ifdef DEBUG_SCOMMAND
					__usart->print("Executing: ");
					__usart->println(__buff);
#endif
					command_list[comm_idx].function(command_list[comm_idx].arg);
				} else {
					__usart->print(__buff);
					__usart->println(" -- What?");
				}
				new_comm = 0;
			}

			void get_next_string(char * str) {
			}

			int get_next_integer(void) {
				int n = 0, s = 1;

				// return if args is empty
				if (__buff[arg_pos] == '\0') return 0;

				// skip delim characters
				while (arg_pos < (COMM_BUFFER_SIZE + 1) && __buff[arg_pos] == delim)
					arg_pos++;

				// deal with signs
				if (__buff[arg_pos] == '-') {
					s = -1;
					arg_pos++;
				}
				if (__buff[arg_pos] == '+') arg_pos++;

				// skip delim characters
				while (arg_pos < (COMM_BUFFER_SIZE + 1) && __buff[arg_pos] == delim)
					arg_pos++;

				// compute integer number
				n = 0;
				while (__buff[arg_pos] >= '0' && __buff[arg_pos] <= '9' && arg_pos < COMM_BUFFER_SIZE + 1) {
					n = n * 10 + (__buff[arg_pos] - '0');
					arg_pos++;
				}

				return n * s;
			}
	};

}

#endif /* SCOMMAND_HPP_ */
