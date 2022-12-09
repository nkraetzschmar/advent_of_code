int32_t check_marker(uint8_t *word)
{
	for (uint64_t i = 0; i < 14; ++i)
	{
		if(!word[i]) return 0;
		for (uint64_t j = i+1; j < 14; ++j)
		{
			if (word[i] == word[j]) return 0;
		}
	}
	return 1;
}

void push_word(uint8_t *word, uint8_t c)
{
	for (uint64_t i = 0; i < 13; ++i) word[i] = word[i+1];
	word[13] = c;
}

void init()
{
	uint8_t buf[0x100];
	uint8_t word[14];

	uint8_t *ptr;
	uint64_t len = 0;
	uint64_t pos = 0;

	while (!check_marker(word))
	{
		if (len == 0)
		{
			len = read(0, buf, sizeof(buf));
			if (!len) exit(1);
			ptr = buf;
		}

		push_word(word, *(ptr++));
		--len;
		++pos;
	}

	uint64_t write_len = format_dec(pos, buf, sizeof(buf));
	if (!write_len) exit(1);
	buf[write_len-1] = '\n';
	write(1, buf, write_len);

	exit(0);
}
