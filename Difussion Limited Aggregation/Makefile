CFLAGS = -Wall -Wextra -Wfloat-equal -pedantic -ansi -lncurses -lm

dla : dla.c neillncurses.c
	$(CC) dla.c neillncurses.c -o dla -g -O $(CFLAGS)

run: all
	./dla

clean:
	rm -f dla

.PHONY: clean run memchk all
