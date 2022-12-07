uint64_t strlen(const uint8_t *buf)
{
	uint64_t len = 0;
	while (*buf)
	{
		++buf;
		++len;
	}
	return len;
}
