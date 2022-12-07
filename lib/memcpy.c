void memcpy(const uint8_t *src, uint8_t *dst, uint64_t len)
{
	while (len)
	{
		*(dst++) = *(src++);
		--len;
	}
}
