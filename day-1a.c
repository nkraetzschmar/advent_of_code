void init()
{
	uint32_t max = 0;
	uint32_t sum = 0;

	uint8_t line[0x100];
	linebuf *f = linebuf_open(0);

	while (!linebuf_eof(f) && !linebuf_error(f))
	{
		uint64_t len = linebuf_read(f, line, sizeof(line)) - 1;
		if (len)
		{
			int32_t value;
			int32_t error = parse_dec(line, &value);
			if (error) exit(1);

			sum += value;
		} else {
			if (sum > max) max = sum;
			sum = 0;
		}
	}

	linebuf_free(f);

	uint64_t len = format_dec(max, line, sizeof(line));
	if (!len) exit(1);
	line[len-1] = '\n';
	write(1, line, len);

	exit(0);
}
