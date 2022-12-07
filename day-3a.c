uint32_t priority(uint8_t c)
{
	uint8_t base;
	uint32_t base_priority;

	if (c < 'a')
	{
		base = 'A';
		base_priority = 27;
	}
	else
	{
		base = 'a';
		base_priority = 1;
	}

	return (c - base) + base_priority;
}

void init()
{
	uint32_t sum = 0;
	uint8_t buf[0x400];

	linebuf *f = linebuf_open(0);
	while (!linebuf_eof(f) && !linebuf_error(f))
	{
		uint64_t len = linebuf_read(f, buf, sizeof(buf));
		if (!len) continue;
		len = (len - 1) / 2;
		for (uint64_t i = 0; i < len; ++i)
		{
			uint8_t c = buf[i];
			if (findchar(c, buf + len))
			{
				sum += priority(c);
				break;
			}
		}
	}
	uint32_t error = linebuf_error(f);
	linebuf_free(f);
	if (error) exit(1);

	uint64_t len = format_dec(sum, buf, sizeof(buf));
	if (!len) exit(1);
	buf[len-1] = '\n';
	write(1, buf, len);

	exit(0);
}
