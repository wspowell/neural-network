
WARNINGS := -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual \
        -Wstrict-prototypes -Wmissing-prototypes

CC := gcc
CFLAGS := -g -O3 -Iinclude -std=c99
LDFLAGS := -lm -pg

LIBNEURO_SRCS := $(wildcard libneuro/*.c)
LIBNEURO_OBJS := $(subst .c,.o,$(LIBNEURO_SRCS))

OBJS += $(LIBNEURO_OBJS)
OBJS += main.o

.PHONY: all clean
.SILENT:

all: clean libneuro.a main

main: main.o libneuro.a
	$(CC) -o $@ $^ $(LDFLAGS)

libneuro.a: $(LIBNEURO_OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean: 
	rm -rf $(OBJS)
	rm -rf libneuro.a
	rm -rf main
	rm -rf gmon.out
