void init()
{
	uint32_t total = 0;
	uint8_t buf[0x10];

	linebuf *f = linebuf_open(0);
	while (!linebuf_eof(f) && !linebuf_error(f))
	{
		uint64_t len = linebuf_read(f, buf, sizeof(buf));
		if (len < 3) continue;

		int32_t shape[2];
		shape[0] = buf[0] - 'A';
		shape[1] = buf[2] - 'X';

		int32_t outcome;
		if (shape[0] == shape[1]) outcome = 3;
		else if ((shape[0] + 1) % 3 == shape[1]) outcome = 6;
		else outcome = 0;

		int32_t score = shape[1] + 1 + outcome;
		total += score;
	}
	linebuf_free(f);

	uint64_t len = format_dec(total, buf, sizeof(buf));
	if (!len) exit(1);
	buf[len-1] = '\n';
	write(1, buf, len);

	exit(0);
}
