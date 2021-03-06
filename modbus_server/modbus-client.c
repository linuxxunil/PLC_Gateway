#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "modbus.h"
#include "modbus-tcp.h"
#include "unit-test.h"


void test_function(modbus_t *ctx, int i, uint8_t *tab_rp_bits, uint16_t *tab_rp_registers)
{
	static uint8_t value;
	static int rc, nb_points;

	switch (i) {
	case 0:	// test : write singal coil (DO)
		printf("[0] Test: write_signal_coil(%d)\n",value);
		value = OFF;
		rc = modbus_write_bit(ctx, UT_BITS_ADDRESS, value);
		
		if (rc == 1) {
			printf("[0] OK\n");
		} else {
			printf("[0] FAILED %d\n",rc);
		}
		
	break;	
	case 1: // test : read coil (DO)
		printf("[1] Test: Read_coil\n");
		rc = modbus_read_bits(ctx, UT_BITS_ADDRESS, 1, tab_rp_bits);
		
		if (rc != 1) {
			printf("[1] FAILED (nb points %d)\n", rc);
		} else {
			i = 0;
			nb_points = UT_INPUT_BITS_NB;
			while (nb_points > 0) {
				int nb_bits = (nb_points > 8) ? 8 : nb_points;
				value = modbus_get_byte_from_bits(tab_rp_bits, i*8, nb_bits);
				printf("[1] Value = %d\n",value);
				nb_points -= nb_bits;
				i++;
			}
			printf("[1] OK\n");
		}
	break;
	
	case 2: // test : read inputs (DI)
	    /** DISCRETE INPUTS **/
		printf("[2] Test: Read discrete inputs\n");
		
		rc = modbus_read_input_bits(ctx, UT_INPUT_BITS_ADDRESS,UT_INPUT_BITS_NB,tab_rp_bits);
		

		if (rc != UT_INPUT_BITS_NB) {
			printf("[2] FAILED (nb points %d)\n", rc);
			break;
		}

		i = 0;
		nb_points = UT_INPUT_BITS_NB;
		while (nb_points > 0) {
			int nb_bits = (nb_points > 8) ? 8 : nb_points;
			value = modbus_get_byte_from_bits(tab_rp_bits, i*8, nb_bits);
			printf("[2] Value = %d\n",value);
			nb_points -= nb_bits;
			i++;
		}
		printf("[2] OK\n");
	break;
	case 3: // test : read input register(AI)
		printf("[3] Test: Read Input Registers\n");
		rc = modbus_read_input_registers(ctx, UT_INPUT_REGISTERS_ADDRESS,UT_INPUT_REGISTERS_NB , tab_rp_registers);
		
		printf("[3] Value = 0x%04x\n",tab_rp_registers[0]);
		printf("[3] OK\n");
		
	break;
	case 4: // test : Write input register (AO)
		printf("[4] Test: Write Input Register\n");
		rc = modbus_write_register(ctx, UT_REGISTERS_ADDRESS, 0);
		
		printf("[4] Value = 0x%04x\n",tab_rp_registers[0]);
		printf("[4] OK\n");
		
	break;
	case 5: // test : Read Holding register (AO)
		printf("[5] Test: Read Holding register\n");
		rc = modbus_read_registers(ctx, UT_REGISTERS_ADDRESS,
											1, tab_rp_registers);
		printf("[5] Value = 0x%04x\n",tab_rp_registers[0]);
		printf("[5] OK\n");
		
	break;
	}

}

enum {
    TCP,
    TCP_PI,
    RTU,
	UNIX_TCP
};

int main(int argc, char*argv[])
{
	int use_backend;
	int test_index, i;
	uint8_t *tab_rp_bits;
	uint16_t *tab_rp_registers;
	modbus_t *ctx;
	
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
            printf("Usage:\n  %s [tcp|tcppi|rtu|unix] - Modbus client for unit testing\n\n", argv[0]);
            return -1;
        }
    } else {
        /* By default */
        use_backend = UNIX_TCP;
    }

    if (use_backend == TCP) {
        ctx = modbus_new_tcp("127.0.0.1", 502);
    } else if (use_backend == UNIX_TCP) {
		ctx = modbus_new_unix_tcp("/tmp/modbus.sock");
	} else if (use_backend == TCP_PI) {
        ctx = modbus_new_tcp_pi("::0", "1502");
    } else {
        ctx = modbus_new_rtu("/dev/ttyUSB0", 115200, 'N', 8, 1);
    }
	
	modbus_set_slave(ctx, 0x64);
	modbus_set_debug(ctx, TRUE);
	modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);

	/* Allocate and initialize the memory to store the bits */
    tab_rp_bits = (uint8_t *) malloc(UT_BITS_NB * sizeof(uint8_t));
    memset(tab_rp_bits, 0, UT_BITS_NB * sizeof(uint8_t));
	
    tab_rp_registers = (uint16_t *) malloc(UT_INPUT_REGISTERS_NB * sizeof(uint16_t));
    memset(tab_rp_registers, 0, UT_INPUT_REGISTERS_NB * sizeof(uint16_t));

	
	test_index = 6;
	for(i=0; i<test_index; i++) {
		if (modbus_connect(ctx) == -1) {
			fprintf(stderr, "Connection failed: %s\n",
					modbus_strerror(errno));
			modbus_free(ctx);
			return -1;
		}
	
		test_function(ctx, i, tab_rp_bits, tab_rp_registers);
		
		modbus_close(ctx);
	}
		


close:
    /* Free the memory */
    free(tab_rp_bits);
    free(tab_rp_registers);
    modbus_free(ctx);

    return 0;
}
