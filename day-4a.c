void init()
{
	uint32_t count = 0;
	uint8_t buf[0x100];

	linebuf *f = linebuf_open(0);
	while (!linebuf_eof(f) && !linebuf_error(f))
	{
		uint64_t len = linebuf_read(f, buf, sizeof(buf));
		if (len <= 1) continue;

		uint8_t *start = buf;
		for (uint64_t i = 0; i < 3; ++i)
		{
			start = findchar("-,"[i%2], start);
			*start = 0;
			++start;
		}

		int32_t values[4];
		start = buf;
		for (uint64_t i = 0; i < 4; ++i)
		{
			parse_dec(start, &values[i]);
			start += strlen(start) + 1;
		}

		if (
			(values[0] <= values[2] && values[1] >= values[3]) ||
			(values[2] <= values[0] && values[3] >= values[1])
		) ++count;
	}
	uint32_t err = linebuf_error(f);
	linebuf_free(f);
	if (err) exit(1);

	uint64_t len = format_dec(count, buf, sizeof(buf));
	if (!len) exit(1);
	buf[len-1] = '\n';
	write(1, buf, len);

	exit(0);
}
