/*
 * Copyright © 2008-2010 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#include "modbus.h"
#include "unit-test.h"
#include "user_callback.h"


/*********** User Callback Definition ***********/
extern modbus_cb_t cb;
/*********** User Callback Definition ***********/


enum {
    TCP,
    TCP_PI,
    RTU,
	UNIX_TCP
};

volatile int running = 1;

static void  signal_handler(int sig_num) {
	printf("Exit Server\n");
	running = 0;
}

int main(int argc, char*argv[])
{
    int socket,client_socket;
	
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc;
    int i;
    int use_backend;
    uint8_t *query;
    int header_length;

    if (argc > 1) {
        if (strcmp(argv[1], "tcp") == 0) {
            use_backend = TCP;
        } else if (strcmp(argv[1], "utcp") == 0 ) {
			use_backend = UNIX_TCP;
		} else if (strcmp(argv[1], "tcppi") == 0) {
            use_backend = TCP_PI;
        } else if (strcmp(argv[1], "rtu") == 0) {
            use_backend = RTU;
        } else {
            printf("Usage:\n  %s [tcp|tcppi|rtu] - Modbus server for unit testing\n\n", argv[0]);
            return -1;
        }
    } else {
        /* By default */
        use_backend = UNIX_TCP;
    }

	//signal(SIGINT, signal_handler);
	
    if (use_backend == TCP) {
        ctx = modbus_new_tcp("127.0.0.1", 502);
        query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);
    } else if (use_backend == UNIX_TCP) {
		ctx = modbus_new_unix_tcp("/tmp/modbus.sock");
        query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);
	} else if (use_backend == TCP_PI) {
        ctx = modbus_new_tcp_pi("::0", "1502");
        query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);
    } else {
        ctx = modbus_new_rtu("/dev/ttyUSB0", 115200, 'N', 8, 1);
        modbus_set_slave(ctx, SERVER_ID);
        query = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
    }
	
	header_length = modbus_get_header_length(ctx);
	modbus_set_debug(ctx, TRUE);
	//by jesse
	modbus_init_cb(ctx, &cb);
	//end

    mb_mapping = modbus_mapping_new(
        UT_BITS_ADDRESS + UT_BITS_NB,	// read buffer : start address + number
        UT_INPUT_BITS_ADDRESS + UT_INPUT_BITS_NB,
        UT_REGISTERS_ADDRESS + UT_REGISTERS_NB,
        UT_INPUT_REGISTERS_ADDRESS + UT_INPUT_REGISTERS_NB);
		
		
    if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    /* Examples from PI_MODBUS_300.pdf.
       Only the read-only input values are assigned. */

    /** INPUT STATUS **/
	
    modbus_set_bits_from_bytes(mb_mapping->tab_input_bits,
                               UT_INPUT_BITS_ADDRESS, UT_INPUT_BITS_NB,
                               UT_INPUT_BITS_TAB);

    /** INPUT REGISTERS **/
    for (i=0; i < UT_INPUT_REGISTERS_NB; i++) {
        mb_mapping->tab_input_registers[UT_INPUT_REGISTERS_ADDRESS+i] =
            UT_INPUT_REGISTERS_TAB[i];;
    }

    if (use_backend == TCP) {
        socket = modbus_tcp_listen(ctx, 1);
    } else if (use_backend == UNIX_TCP) {
		socket = modbus_unix_tcp_listen(ctx, 1);
	} else if (use_backend == TCP_PI) {
        socket = modbus_tcp_pi_listen(ctx, 1);
    } else {
        client_socket = modbus_connect(ctx);
        if (rc == -1) {
            fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
            modbus_free(ctx);
            return -1;
        }
    }
	
	// for unix sock
    while (running) {
		if (use_backend == TCP) {
			client_socket = modbus_tcp_accept(ctx, &socket);
		} else if (use_backend == UNIX_TCP) {
			client_socket = modbus_unix_tcp_accept(ctx, &socket);
		} else if (use_backend == TCP_PI) {
			client_socket = modbus_tcp_pi_accept(ctx, &socket);
		} else {
			// RTU : nothing
		}
	
        rc = modbus_receive(ctx, query);
        if (rc == -1) {
            /* Connection closed by the client or error */
			if ( use_backend != RTU)
				close(client_socket);
            continue;
        }

        /* Read holding registers */
        if (query[header_length] == 0x03) {
            if (MODBUS_GET_INT16_FROM_INT8(query, header_length + 3)
                == UT_REGISTERS_NB_SPECIAL) {
                printf("Set an incorrect number of values\n");
                MODBUS_SET_INT16_TO_INT8(query, header_length + 3,
                                         UT_REGISTERS_NB_SPECIAL - 1);
            } else if (MODBUS_GET_INT16_FROM_INT8(query, header_length + 1)
                == UT_REGISTERS_ADDRESS_SPECIAL) {
                printf("Reply to this special register address by an exception\n");
                modbus_reply_exception(ctx, query,
                                       MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY);
                continue;
            }
        }

        rc = modbus_reply(ctx, query, rc, mb_mapping);
		
		if ( use_backend != RTU)
			close(client_socket);
    }

    printf("Quit the loop: %s\n", modbus_strerror(errno));

    if (use_backend == TCP) {
        close(socket);
    }
	
	modbus_uninit_cb(ctx, &cb);
    modbus_mapping_free(mb_mapping);
    free(query);
    modbus_free(ctx);

    return 0;
}
