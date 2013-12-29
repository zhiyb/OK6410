#ifndef MEM_H
#define MEM_H

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>

extern int mem_fd;
extern void *mem;

#define HWREG(offset) (*(volatile unsigned int *)\
		((volatile unsigned char *)mem + offset))
#define _BV(b)	(1 << (b))

static inline void mem_open(uint32_t base)
{
	mem_fd = open("/dev/mem", O_RDWR);
	if (mem_fd == -1) {
		fputs("Open /dev/mem failed!", stderr);
		_exit(1);
	}
	mem = mmap(NULL, 0x00010000, PROT_READ | PROT_WRITE, \
			MAP_SHARED, mem_fd, base);
	if (mem == NULL) {
		fputs("mmap() failed!", stderr);
		close(mem_fd);
		_exit(1);
	}
}

static inline void mem_close(void)
{
	munmap(mem, 0x00010000);
	close(mem_fd);
}

#endif
