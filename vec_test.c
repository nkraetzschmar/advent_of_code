void init()
{
	vec_uint8_t *vec = vec_uint8_t_alloc();
	if (!vec) exit(1);
	for (uint64_t i = 0; i < 1000000; ++i)
	{
		int32_t err = vec_uint8_t_push(vec, 'A' + (i%26));
		if (err) exit(1);
	}

	uint8_t buf[] = " \n";
	while (vec_uint8_t_len(vec))
	{
		uint8_t x = vec_uint8_t_pop(vec);
		buf[0] = x;
		write(1, buf, 2);
	}

	exit(0);
}
