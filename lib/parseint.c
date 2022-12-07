int32_t parse_dec(const uint8_t *buf, int32_t *value)
{
	int32_t sign = 1;
	if (*buf == '-')
	{
		sign = -1;
		++buf;
	}

	int32_t accumulator = 0;
	while(*buf >= '0' && *buf <= '9') accumulator = (accumulator * 10) + (int32_t) (*(buf++) - '0');
	accumulator *= sign;

	if (*buf == 0)
	{
		*value = accumulator;
		return 0;
	} else {
		return -1;
	}
}
