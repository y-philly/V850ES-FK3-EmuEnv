#include "udp/udp_comm.h"
#include <string.h>
#include <stdio.h>

int main(int argc, const char* argv[])
{
	UdpCommConfigType config;
	UdpCommType comm;
	int len;

	if (argc != 2) {
		printf("Usage: %s command\n", argv[0]);
		return 1;
	}
	len = strlen(argv[1]);
	if (len > UDP_BUFFER_LEN) {
		printf("argument length is too large.len=%d\n", len);
		return 1;
	}
	config.server_port = 49153;
	config.client_port = 49152;

	comm.write_data.len = strlen(argv[1]);
	memcpy(comm.write_data.buffer, argv[1], strlen(argv[1]));
	comm.write_data.buffer[strlen(argv[1])] = '\0';

	if (winsock_init() < 0) {
		printf("ERROR:winsock_init failed.\n");
		return 1;
	}

	if (udp_comm_create(&config, &comm) != STD_E_OK) {
		printf("ERROR:udp_comm_create failed.\n");
		return 1;
	}

	if (udp_comm_write(&comm) != STD_E_OK) {
		printf("ERROR:udp_comm_write failed.\n");
		return 1;
	}
	if (udp_comm_read(&comm) != STD_E_OK) {
		printf("ERROR:udp_comm_read failed.\n");
		return 1;
	}
	winsock_fini();

	printf("%s", comm.read_data.buffer);
	return 0;
}
