#include "vec.h"

struct VEC_STRUCT
{
	uint64_t len;
	uint64_t cap;
	TYPE *buf;
};

static int32_t VEC_FUNCTION(resize)(VEC_STRUCT *vec, uint64_t cap);

VEC_STRUCT * VEC_FUNCTION(alloc)()
{
	VEC_STRUCT *vec = mmap((void*) 0, sizeof(VEC_STRUCT), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!vec) return (void*) 0;
	vec->len = 0;
	vec->cap = INIT_CAP;
	vec->buf = mmap((void*) 0, vec->cap * sizeof(TYPE), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!vec->buf)
	{
		munmap(vec, sizeof(VEC_STRUCT));
	}
	return vec;
}

void VEC_FUNCTION(free)(VEC_STRUCT *vec)
{
	munmap(vec->buf, vec->cap * sizeof(TYPE));
	munmap(vec, sizeof(VEC_STRUCT));
}

uint64_t VEC_FUNCTION(len)(const VEC_STRUCT *vec)
{
	return vec->len;
}

TYPE * VEC_FUNCTION(buf)(const VEC_STRUCT *vec)
{
	return vec->buf;
}

int32_t VEC_FUNCTION(push)(VEC_STRUCT *vec, TYPE x)
{
	uint64_t len = vec->len + 1;
	if (len > vec->cap)
	{
		int32_t err = VEC_FUNCTION(resize)(vec, vec->cap * 2);
		if (err) return -1;
	}
	vec->len = len;
	vec->buf[len-1] = x;
	return 0;
}

TYPE VEC_FUNCTION(pop)(VEC_STRUCT *vec)
{
	--(vec->len);
	TYPE x = vec->buf[vec->len];
	if (vec->len <= vec->cap / 4 && vec->cap / 2 >= INIT_CAP) VEC_FUNCTION(resize)(vec, vec->cap / 2);
	return x;
}

static int32_t VEC_FUNCTION(resize)(VEC_STRUCT *vec, uint64_t cap)
{
	if (vec->len > cap) return -1;
	TYPE *buf = mremap(vec->buf, vec->cap, cap, MREMAP_MAYMOVE, (void*) 0);
	if (!buf) return -1;
	vec->cap = cap;
	vec->buf = buf;
	return 0;
}
