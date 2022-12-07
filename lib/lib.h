#define int8_t char
#define uint8_t unsigned char
#define int32_t int
#define uint32_t unsigned int
#define int64_t long
#define uint64_t unsigned long

uint64_t syscall(uint64_t syscall_num, uint64_t arg_0, uint64_t arg_1, uint64_t arg_2, uint64_t arg_3, uint64_t arg_4, uint64_t arg_5);

uint64_t read(uint32_t fd, uint8_t *buf, uint64_t count);
uint64_t write(uint32_t fd, const uint8_t *buf, uint64_t count);
void exit(int32_t error_code);

#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4

#define MAP_SHARED    0x01
#define MAP_PRIVATE   0x02
#define MAP_ANONYMOUS 0x20

void *mmap(void *addr, uint64_t length, int32_t prot, int32_t flags, int32_t fd, uint64_t offset);
int32_t munmap(void *addr, uint64_t length);

void memcpy(const uint8_t *src, uint8_t *dst, uint64_t len);
uint64_t strlen(const uint8_t *buf);
uint8_t * findchar(uint8_t c, uint8_t *buf);

typedef struct linebuf linebuf;

linebuf * linebuf_open(uint32_t fd);
uint32_t linebuf_eof(linebuf *f);
uint32_t linebuf_error(linebuf *f);
uint64_t linebuf_read(linebuf *f, uint8_t *dst, uint64_t len);
void linebuf_free(linebuf *f);

uint32_t format_dec(int32_t v, uint8_t *dst, uint64_t len);
int32_t parse_dec(const uint8_t *buf, int32_t *value);
