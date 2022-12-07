void init()
{
	int32_t num;
	uint32_t error = parse_dec((uint8_t*) "7", &num);
	if (error) exit(1);

	uint8_t buf[5];
	uint64_t len = format_dec(num, buf, sizeof(buf));
	if (!len) exit(1);

	buf[len-1] = '\n';
	write(1, buf, len);
	exit(0);
}
