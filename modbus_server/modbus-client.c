#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "modbus.h"
#include "modbus-tcp.h"
#include "unit-test.h"


void test_function(modbus_t *ctx, int i, uint8_t *tab_rp_bits)
{
	int rc;
	switch (i) {
	case 0:
			/* Unit Test */
		rc = modbus_write_bit(ctx, UT_BITS_ADDRESS, ON);
		printf("1/2 modbus_write_bit: ");
		if (rc == 1) {
			printf("OK\n");
		} else {
			printf("FAILED %d\n",rc);
		}
		
	break;	
	case 1:
	
		rc = modbus_read_bits(ctx, UT_BITS_ADDRESS, 1, tab_rp_bits);
		printf("2/2 modbus_read_bits: ");
		if (rc != 1) {
			printf("FAILED (nb points %d)\n", rc);
		} else {
			printf("OK\n");
		}
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

	
	test_index = 2;
	for(i=0; i<test_index; i++) {
		if (modbus_connect(ctx) == -1) {
			fprintf(stderr, "Connection failed: %s\n",
					modbus_strerror(errno));
			modbus_free(ctx);
			return -1;
		}
	
		test_function(ctx, i, tab_rp_bits);
		
		modbus_close(ctx);
	}
		


close:
    /* Free the memory */
    free(tab_rp_bits);
    //free(tab_rp_registers);
    modbus_free(ctx);

    return 0;
}
