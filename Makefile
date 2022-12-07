.SUFFIXES:
.PHONY: all clean

AS=aarch64-linux-gnu-as
CC=aarch64-linux-gnu-gcc
LD=aarch64-linux-gnu-ld
AR=aarch64-linux-gnu-ar

AS-FLAGS=-Wall -Wextra -Werror
CC_FLAGS=-Os -std=c11 -nostdinc -include lib/lib.h -fno-builtin -fno-optimize-sibling-calls -fomit-frame-pointer -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -Wall -Wextra -Werror
LD_FLAGS=-s -static -nostdlib

OBJECTS=$(patsubst %.c,%.o,$(wildcard *.c) $(wildcard lib/*.c)) $(patsubst %.s,%.o,$(wildcard *.s) $(wildcard lib/*.s))
BINARIES=true hello echo num $(patsubst %.c,%,$(wildcard day-*.c))

all: $(BINARIES)
clean:
	rm -f libutil.a $(OBJECTS) $(BINARIES)

lib/libutil.a: $(filter lib/%.o,$(OBJECTS))

$(BINARIES): %: %.o lib/libutil.a
	$(LD) $(LD_FLAGS) -o $@ $< -Llib -lutil
	objcopy --remove-section=.comment --remove-section=.note $@ $@

%.o: %.c lib/lib.h
	$(CC) $(CC_FLAGS) -o $@ -c $<

%.o: %.s
	$(AS) $(AS_FLAGS) -o $@ -c $<

%.a:
	$(AR) -rcs $@ $^
