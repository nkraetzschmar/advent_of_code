uint64_t read(uint32_t fd, uint8_t *buf, uint64_t count)
{
	return syscall(0x3f, (uint64_t) fd, (uint64_t) buf, count, 0, 0, 0);
}
