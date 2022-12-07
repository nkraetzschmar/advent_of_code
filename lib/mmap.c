void *mmap(void *addr, uint64_t length, int32_t prot, int32_t flags, int32_t fd, uint64_t offset)
{
	return (void*) syscall(0xde, (uint64_t) addr, length, prot, flags, fd, offset);
}

int32_t munmap(void *addr, uint64_t length)
{
	return syscall(0xd7, (uint64_t) addr, length, 0, 0, 0, 0);
}
