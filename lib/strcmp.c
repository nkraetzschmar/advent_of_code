int32_t strcmp(const uint8_t *a, const uint8_t *b)
{
	while (*a) if (*(a++) != *(b++)) return 0;
	return 1;
}
