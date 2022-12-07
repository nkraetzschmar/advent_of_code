uint8_t * findchar(uint8_t c, uint8_t *buf)
{
	while (*buf)
	{
		if (*buf == c) return buf;
		++buf;
	}
	return (void*) 0;
}
