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

		int32_t outcome = buf[2] - 'Y';
		shape[1] = (shape[0] + 3 + outcome) % 3;

		int32_t score = shape[1] + 1 + ((outcome + 1) * 3);
		total += score;
	}
	linebuf_free(f);

	uint64_t len = format_dec(total, buf, sizeof(buf));
	if (!len) exit(1);
	buf[len-1] = '\n';
	write(1, buf, len);

	exit(0);
}
