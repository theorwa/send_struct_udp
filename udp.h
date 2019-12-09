void DieWithError(char *err);

struct listen_package_t
{
	uint32_t magic;
	uint16_t port_number;
	char shell_command[256];
};

extern struct listen_package_t udp_packet;
