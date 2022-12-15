PLATFORM?=x64
ROLLFORWARD_PATH?=../
ROLLFORWARD_SRC_PATH=$(ROLLFORWARD_PATH)/src/$(PLATFORM)
CC?=gcc
CFLAGS=-I $(ROLLFORWARD_SRC_PATH) -fno-stack-protector
RF_COMPILER=$(ROLLFORWARD_SRC_PATH)/transform.pl

rollforward.o: $(ROLLFORWARD_SRC_PATH)/rollforward.c $(ROLLFORWARD_SRC_PATH)/rollforward.h
	$(CC) -c -o $@ $< $(CFLAGS)

sum_array.s: sum_array.c $(ROLLFORWARD_SRC_PATH)/rollforward.h
	$(CC) -S -o $@ $< $(CFLAGS)

sum_array.rf.s: sum_array.s
	$(RF_COMPILER) $< $@

sum_array: sum_array.rf.s rollforward.o
	$(CC) sum_array.rf.s -pthread -o $@ $(CFLAGS) rollforward.o

clean:
	rm -f *.o *.s
