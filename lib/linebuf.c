#define BUF_LEN 0x400

struct linebuf
{
	uint32_t fd;
	uint32_t eof;
	uint32_t error;
	uint8_t buf[BUF_LEN];
};

linebuf * linebuf_open(uint32_t fd)
{
	linebuf *f = mmap((void*) 0, sizeof(linebuf), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	f->fd = fd;
	return f;
}

uint32_t linebuf_eof(linebuf *f)
{
	return f->eof;
}

uint32_t linebuf_error(linebuf *f)
{
	return f->error;
}

uint64_t linebuf_read(linebuf *f, uint8_t *dst, uint64_t len)
{
	if (f->error) return 0;

	uint8_t *end_of_line = findchar('\n', f->buf);

	uint64_t offset = strlen(f->buf);
	while (!end_of_line)
	{
		if (offset == BUF_LEN)
		{
			f->error = 1;
			return 0;
		}

		uint64_t read_len = read(f->fd, f->buf + offset, BUF_LEN - offset);
		if (read_len == 0)
		{
			f->eof = 1;
			return 0;
		}

		end_of_line = findchar('\n', f->buf + offset);
		offset += read_len;
	}

	*end_of_line = '\0';
	uint64_t write_len = (end_of_line - f->buf) + 1;
	if (write_len > len)
	{
		f->error = 1;
		return 0;
	}

	memcpy(f->buf, dst, write_len);
	memcpy(end_of_line + 1, f->buf, BUF_LEN - write_len);
	for (uint64_t i = BUF_LEN - write_len; i < BUF_LEN; ++i) f->buf[i] = 0;

	return write_len;
}

void linebuf_free(linebuf *f)
{
	munmap(f, sizeof(linebuf));
}
