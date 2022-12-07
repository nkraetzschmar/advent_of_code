uint32_t format_dec(int32_t v, uint8_t *dst, uint64_t len)
{
	uint8_t _buf[0x10] = "0";
	uint8_t *buf = _buf;
	if (v < 0) *(buf++) = '-';
	for (int i = v; i; i /= 10) ++buf;
	while (v)
	{
		*(--buf) = '0' + (v % 10);
		v /= 10;
	}

	uint64_t write_len = strlen(buf) + 1;
	if (write_len < len)
	{
		memcpy(buf, dst, write_len);
		return write_len;
	} else return 0;
}
