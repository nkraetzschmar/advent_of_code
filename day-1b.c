void init()
{
	uint32_t max[3] = { 0, 0, 0 };
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
			for (uint64_t i = 0; i < 3; ++i)
			{
				if (sum > max[i])
				{
					for (uint64_t j = 2; j > i; --j) max[j] = max[j-1];
					max[i] = sum;
					break;
				}
			}
			sum = 0;
		}
	}

	linebuf_free(f);

	for (uint64_t i = 0; i < 3; ++i)
	{
		uint64_t len = format_dec(max[i], line, sizeof(line));
		if (!len) exit(1);
		line[len-1] = '\n';
		write(1, line, len);
	}

	uint32_t max_sum = 0;
	for (uint64_t i = 0; i < 3; ++i) max_sum += max[i];
	uint64_t len = format_dec(max_sum, line, sizeof(line));
	if (!len) exit(1);
	line[len-1] = '\n';
	write(1, line, len);

	exit(0);
}
