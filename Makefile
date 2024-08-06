CFLAGS =-Wall -Wextra -O0  -Ilib -g
LDFLAGS = -lssl  -lcrypto -lm

Test: set.c
	set xe
	cc $(CFLAGS) -o test set.c $(LDFLAGS)

.PHONY: test clean

test: Test
	./test

clean:
	rm -f Test

