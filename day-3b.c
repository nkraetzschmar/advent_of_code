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
	uint8_t lines[3][0x400];

	linebuf *f = linebuf_open(0);
	while (!linebuf_eof(f) && !linebuf_error(f))
	{
		for (uint64_t i = 0; i < 3; ++i)
		{
			lines[i][0] = 0;
			linebuf_read(f, lines[i], sizeof(lines[0]));
		}

		uint64_t len = strlen(lines[0]);
		for (uint64_t i = 0; i < len; ++i)
		{
			uint8_t c = lines[0][i];
			if (findchar(c, lines[1]) && findchar(c, lines[2]))
			{
				sum += priority(c);
				break;
			}
		}
	}
	uint32_t error = linebuf_error(f);
	linebuf_free(f);
	if (error) exit(1);

	uint64_t len = format_dec(sum, lines[0], sizeof(lines[0]));
	if (!len) exit(1);
	lines[0][len-1] = '\n';
	write(1, lines[0], len);

	exit(0);
}
