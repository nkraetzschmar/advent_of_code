void print_stacks(vec_uint8_t **stacks, uint64_t num_stacks)
{
	uint64_t max_len = 0;
	for (uint64_t i = 0; i < num_stacks; ++i)
	{
		uint64_t len = vec_uint8_t_len(stacks[i]);
		if (len > max_len) max_len = len;
	}

	uint8_t buf[num_stacks * 4];
	for (uint64_t i = 0; i < num_stacks * 4 - 1; ++i) buf[i] = ' ';
	buf[num_stacks * 4 - 1] = '\n';

	for (uint64_t i = max_len; i > 0; --i)
	{
		for (uint64_t j = 0; j < num_stacks; ++j)
		{
			if (vec_uint8_t_len(stacks[j]) >= i)
			{
				buf[(j*4)+0] = '[';
				buf[(j*4)+1] = vec_uint8_t_buf(stacks[j])[i-1];
				buf[(j*4)+2] = ']';
			}
		}
		write(1, buf, num_stacks * 4);
	}
}

void init()
{
	const uint64_t num_stacks = 0x10;
	vec_uint8_t *stacks[num_stacks];
	vec_uint8_t *inverted_stacks[num_stacks];
	vec_uint8_t *tmp_stack = vec_uint8_t_alloc();
	if (!tmp_stack) exit(1);

	for (uint64_t i = 0; i < num_stacks; ++i)
	{
		stacks[i] = vec_uint8_t_alloc();
		inverted_stacks[i] = vec_uint8_t_alloc();
		if (!stacks[i] || !inverted_stacks[i]) exit(1);
	}

	uint8_t buf[num_stacks * 4];
	linebuf *f = linebuf_open(0);
	while (!linebuf_eof(f) && !linebuf_error(f))
	{
		uint64_t len = linebuf_read(f, buf, sizeof(buf));
		if (len <= 1) break;

		for (uint64_t i = 0; i < len / 4; ++i)
		{
			if (!(buf[i*4] == '[' && buf[(i*4)+2] == ']')) continue;
			vec_uint8_t_push(inverted_stacks[i], buf[(i*4)+1]);
		}
	}
	if (linebuf_error(f)) exit(1);

	for (uint64_t i = 0; i < num_stacks; ++i)
	{
		uint64_t len = vec_uint8_t_len(inverted_stacks[i]);
		for (uint64_t j = 0; j < len; ++j)
		{
			vec_uint8_t_push(stacks[i], vec_uint8_t_pop(inverted_stacks[i]));
		}
	}

	print_stacks(stacks, num_stacks);

	while (!linebuf_eof(f) && !linebuf_error(f))
	{
		uint64_t len = linebuf_read(f, buf, sizeof(buf));
		if (!len) break;

		buf[len-1] = '\n';
		write(1, buf, len);

		uint8_t *start;
		uint8_t *end;

		start = findchar(' ', buf) + 1;
		end = findchar(' ', start);
		*end = 0;
		int32_t mul;
		parse_dec(start, &mul);
		start = end + 1;

		start = findchar(' ', start) + 1;
		end = findchar(' ', start);
		*end = 0;
		int32_t from;
		parse_dec(start, &from);
		start = end + 1;

		start = findchar(' ', start) + 1;
		end = findchar('\n', start);
		*end = 0;
		int32_t to;
		parse_dec(start, &to);

		for (int32_t i = 0; i < mul; ++i)
		{
			vec_uint8_t_push(tmp_stack, vec_uint8_t_pop(stacks[from-1]));
		}

		for (int32_t i = 0; i < mul; ++i)
		{
			vec_uint8_t_push(stacks[to-1], vec_uint8_t_pop(tmp_stack));
		}

		print_stacks(stacks, num_stacks);
	}
	if (linebuf_error(f)) exit(1);

	linebuf_free(f);

	uint8_t *ptr = buf;
	uint64_t len = 1;
	for (uint64_t i = 0; i < num_stacks; ++i)
	{
		if (vec_uint8_t_len(stacks[i]))
		{
			*(ptr++) = vec_uint8_t_pop(stacks[i]);
			++len;
		}
	}
	*ptr = '\n';
	write(1, buf, len);

	exit(0);
}
