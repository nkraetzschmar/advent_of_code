uint64_t write(uint32_t fd, const uint8_t *buf, uint64_t count)
{
	return syscall(0x40, (uint64_t) fd, (uint64_t) buf, count, 0, 0, 0);
}
