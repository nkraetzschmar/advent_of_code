.SUFFIXES:
.PHONY: all clean

AS=aarch64-linux-gnu-as
CC=aarch64-linux-gnu-gcc
LD=aarch64-linux-gnu-ld
AR=aarch64-linux-gnu-ar
CPP=aarch64-linux-gnu-cpp

CC_FLAGS=-Os -std=c11 -nostdinc -include lib/lib.h -include lib/vec.out.h -fno-builtin -fno-optimize-sibling-calls -fomit-frame-pointer -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -Wall -Wextra -Werror
LD_FLAGS=-s -static -nostdlib
CPP_FLAGS=-nostdinc

VEC_TYPES=int8_t uint8_t int32_t uint32_t int64_t uint64_t
VEC_OBJECTS=$(addsuffix .o,$(addprefix lib/vec.,$(VEC_TYPES)))
VEC_HEADERS=$(addsuffix .h,$(addprefix lib/vec.,$(VEC_TYPES)))
OBJECTS=$(patsubst %.c,%.o,$(wildcard *.c) $(filter-out lib/vec.c,$(wildcard lib/*.c))) $(patsubst %.s,%.o,$(wildcard *.s) $(wildcard lib/*.s)) $(VEC_OBJECTS)
BINARIES=true hello echo num vec_test $(patsubst %.c,%,$(wildcard day-*.c))

all: $(BINARIES)
clean:
	rm -f lib/libutil.a $(OBJECTS) $(BINARIES) lib/vec.out.h $(VEC_HEADERS)

lib/libutil.a: $(filter lib/%.o,$(OBJECTS))

$(BINARIES): %: %.o lib/libutil.a
	$(LD) $(LD_FLAGS) -o $@ $< -Llib -lutil
	objcopy --remove-section=.comment --remove-section=.note $@ $@

%.o: %.c lib/lib.h lib/vec.out.h
	$(CC) $(CC_FLAGS) -o $@ -c $<

%.o: %.s
	$(AS) -o $@ -c $<

%.a:
	$(AR) -rcs $@ $^

lib/vec.%.h: lib/vec.h
	$(CPP) $(CPP_FLAGS) -DTYPE=$* $< > $@

lib/vec.out.h: $(VEC_HEADERS)
	cat $^ > $@

lib/vec.%.o: lib/vec.c lib/lib.h
	$(CC) $(CC_FLAGS) -DTYPE=$* -o $@ -c $<
