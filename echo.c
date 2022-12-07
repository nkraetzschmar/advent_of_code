void init() {
	uint8_t line[0x100];
	linebuf *f = linebuf_open(0);
	while (!linebuf_eof(f) && !linebuf_error(f))
	{
		uint64_t len = linebuf_read(f, line, sizeof(line));
		line[len-1] = '\n';
		write(1, line, len);
	}
	int32_t ret_code = linebuf_error(f);
	linebuf_free(f);
	exit(ret_code);
}
