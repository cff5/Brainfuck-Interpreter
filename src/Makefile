
CC=gcc

CFLAGS=-Ofast -Wno-unused-result

brainf: *.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o brainf