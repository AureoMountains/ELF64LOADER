CC := gcc
NASM := nasm

CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g
LDFLAGS := -Wl,-Map=elfinspect.map

.PHONY: all clean inspect qemu

all: elfinspect

elfinspect.o: elfinspect.c
	$(CC) $(CFLAGS) -c $< -o $@

elfinspect: elfinspect.o
	$(CC) $(LDFLAGS) $^ -o $@

inspect: elfinspect
	./elfinspect ./elfinspect

qemu: elfinspect
	qemu-x86_64 ./elfinspect ./elfinspect

clean:
	rm -f elfinspect elfinspect.o elfinspect.map
