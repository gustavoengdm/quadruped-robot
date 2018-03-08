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

namespace scommand {

	template<uint8_t NBR_OF_COMMANDS, uint8_t SERIAL_COMMANDER_BUFFER_SIZE> class SCommand {

		public:
			USART * __usart;
			char buff[SERIAL_COMMANDER_BUFFER_SIZE];
			uint8_t comm_count;
			char endchar;

			typedef struct {
					char * msg;
					void (*function)(void);
			} Command;
			Command commands[NBR_OF_COMMANDS];

			volatile uint8_t new_command;

			uint8_t buff_counter;
			uint8_t buff_start;

		public:

			SCommand(USART &us, char ch = '\n') :
					__usart(&us), comm_count(0), endchar(ch), //
					new_command(0), buff_counter(0), buff_start(0) {
			}

			void add(const char * msg, void (*f)(void)) {
				int sz;
				sz = strlen(msg);
				if (comm_count < NBR_OF_COMMANDS && sz < SERIAL_COMMANDER_BUFFER_SIZE) {
					commands[comm_count].msg = new char[sz];
					strcpy(commands[comm_count].msg, msg);
					commands[comm_count].function = f;
					comm_count++;
				}
			}

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
					__usart->println("serial_rx_interrupt: end of command");
					//(commands[0].function)();

					return;
				}
				buff_counter++;
			}

			void proc() {
				if (!new_command) return;
				new_command = 0;
				commands[0].function();
			}
	};

	template<uint8_t NBR_OF_COMMANDS, uint8_t SERIAL_COMMANDER_BUFFER_SIZE>
	void link(SCommand<NBR_OF_COMMANDS, SERIAL_COMMANDER_BUFFER_SIZE> * com, USART * us) {
		//us->attach_rx_interrupt(com->serial_rx_interrupt);
	}

}

#endif /* SCOMMAND_HPP_ */
