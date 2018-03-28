CFLAGS = -Wall -Wextra -Wfloat-equal -pedantic -ansi
INCS = bst.h

all: testbst spl testbst_d spl_d

testbst:  bst.c testbst.c $(INCS)
	$(CC) bst.c testbst.c -o testbst -O3 $(CFLAGS)

spl : spl.c bst.c  $(INCS)
	$(CC) bst.c spl.c -o spl -O3 $(CFLAGS)

testbst_d:  bst.c testbst.c $(INCS)
	$(CC) bst.c testbst.c -o testbst_d -g -O $(CFLAGS)

spl_d : spl.c bst.c  $(INCS)
	$(CC) bst.c spl.c -o spl_d -g -O $(CFLAGS)

run: all
	./testbst
	./spl eng_370k_shuffle.txt heart_darkness.txt

memchk: testbst_d spl_d
	valgrind --error-exitcode=1 --quiet --leak-check=full ./testbst_d
	valgrind --error-exitcode=1 --quiet --leak-check=full ./spl_d eng_370k_shuffle.txt heart_darkness.txt

clean:
	rm -f testbst testbst_d spl spl_d

.PHONY: clean run memchk all
