void exit(int32_t error_code)
{
	syscall(0x5d, error_code, 0, 0, 0, 0, 0);
}
