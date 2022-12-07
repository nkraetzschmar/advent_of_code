const uint8_t msg[] = "hello ";

void init(__attribute__((unused)) uint64_t argc, uint8_t **argv)
{
	const uint64_t len_buf = 0x100;
	uint8_t buf[len_buf];
	const uint64_t len_msg = sizeof(msg) - 1;

	if (argc != 2) exit(1);
	uint64_t len_arg = strlen(argv[1]);
	if (len_msg + len_arg + 1 > len_buf) exit(1);

	memcpy(msg, buf, len_msg);
	memcpy(argv[1], buf + len_msg, len_arg);
	buf[len_msg + len_arg] = '\n';
	write(1, buf, len_msg + len_arg + 1);

	exit(0);
}
