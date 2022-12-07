void *mmap(void *addr, uint64_t length, int32_t prot, int32_t flags, int32_t fd, uint64_t offset)
{
	return (void*) syscall(0xde, (uint64_t) addr, length, prot, flags, fd, offset);
}

int32_t munmap(void *addr, uint64_t length)
{
	return syscall(0xd7, (uint64_t) addr, length, 0, 0, 0, 0);
}

void *mremap(void *old_addr, uint64_t old_len, uint64_t new_len, int32_t flags, void* new_addr)
{
	return (void*) syscall(0xd8, (uint64_t) old_addr, old_len, new_len, flags, (uint64_t) new_addr, 0);
}
