#include "udp_comm.h"


Std_ReturnType udp_comm_create(const UdpCommConfigType *config, UdpCommType *comm)
{
	int err;
	struct sockaddr_in addr;

	err = socket(AF_INET, SOCK_DGRAM, 0);
	if (err < 0) {
		return STD_E_INVALID;
	}
	comm->srv_sock = err;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(config->server_port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	err = bind(comm->srv_sock, (struct sockaddr *)&addr, sizeof(addr));
	if (err < 0) {
		return STD_E_INVALID;
	}

	comm->client_port = htons(config->client_port);

	return STD_E_OK;
}

Std_ReturnType udp_comm_read(UdpCommType *comm)
{
	int err;

	err = recv(comm->srv_sock, comm->read_data.buffer, sizeof(comm->read_data.buffer), 0);
	if (err <= 0) {
		return STD_E_INVALID;
	}
	comm->read_data.len = err;

	return STD_E_OK;
}

Std_ReturnType udp_comm_write(UdpCommType *comm)
{
	int err;
	struct sockaddr_in addr;

	err = socket(AF_INET, SOCK_DGRAM, 0);
	if (err < 0) {
		return STD_E_INVALID;
	}
	comm->clt_sock = err;

	addr.sin_family = AF_INET;
	addr.sin_port = comm->client_port;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	err = sendto(comm->clt_sock, comm->write_data.buffer, comm->write_data.len, 0,
			(struct sockaddr *)&addr, sizeof(addr));
	if (err != comm->write_data.len) {
		return STD_E_INVALID;
	}
	closesocket(comm->clt_sock);
	comm->clt_sock = -1;

	return STD_E_OK;
}
void udp_server_delete(UdpCommType *comm)
{
	closesocket(comm->srv_sock);
	comm->srv_sock = -1;
	return;
}
