int32_t visible(uint8_t **map, uint64_t width, uint64_t height, uint64_t x, uint64_t y)
{
	uint8_t point_height = map[y][x];
	int32_t direction_visible;
	direction_visible = 1;
	for (uint64_t i = 0; i < x; ++i)
	{
		if (map[y][i] >= point_height)
		{
			direction_visible = 0;
			break;
		}
	}
	if (direction_visible) return 1;
	direction_visible = 1;
	for (uint64_t i = x+1; i < width; ++i)
	{
		if (map[y][i] >= point_height)
		{
			direction_visible = 0;
			break;
		}
	}
	if (direction_visible) return 1;
	direction_visible = 1;
	for (uint64_t i = 0; i < y; ++i)
	{
		if (map[i][x] >= point_height)
		{
			direction_visible = 0;
			break;
		}
	}
	if (direction_visible) return 1;
	direction_visible = 1;
	for (uint64_t i = y+1; i < height; ++i)
	{
		if (map[i][x] >= point_height)
		{
			direction_visible = 0;
			break;
		}
	}
	if (direction_visible) return 1;
	return 0;
}

void init()
{
	uint8_t *buf;
	uint64_t cap = 0x100;
	uint64_t len = 0;
	buf = mmap((void*) 0, 0x100, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!buf) exit(1);

	while (1)
	{
		if (cap - len < cap / 2)
		{
			buf = mremap(buf, cap, cap * 2, MREMAP_MAYMOVE, (void*) 0);
			if (!buf) exit(1);
			cap *= 2;
		}

		uint64_t read_len = read(0, buf + len, cap - len);
		if (!read_len) break;
		len += read_len;
	}

	uint8_t *line_end = findchar('\n', buf);
	if (!line_end) exit(1);
	uint64_t line_len = (line_end - buf) + 1;
	uint64_t num_lines = len / line_len;
	if (len % line_len) exit(1);

	uint8_t **map = mmap((void*) 0, sizeof(void*) * num_lines, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!map) exit(1);
	for (uint64_t i = 0; i < num_lines; ++i) map[i] = buf + (i * line_len);

	uint64_t count = 0;
	for (uint64_t x = 0; x < line_len - 1; ++x)
	{
		for (uint64_t y = 0; y < num_lines; ++y)
		{
			count += visible(map, line_len - 1, num_lines, x, y);
		}
	}

	uint8_t out_buf[0x100];
	uint64_t out_len = format_dec(count, out_buf, sizeof(out_buf));
	if (!out_len) exit(1);
	out_buf[out_len-1] = '\n';
	write(1, out_buf, out_len);

	exit(0);
}
