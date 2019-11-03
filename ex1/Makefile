CC=gcc -Wall

PROGS=read_string_fifo write_string_fifo

all: $(PROGS)

clean:
	rm -f $(PROGS)

read_fifo:	read_fifo.c
	$(CC) read_string_fifo.c -o read_string_fifo

write_fifo:	write_fifo.c
	$(CC) write_string_fifo.c -o write_string_fifo
