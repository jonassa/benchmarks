all: *.c *.h
	gcc *.c -o loader -O3 -Wall -levent -pthread -lm -D_GNU_SOURCE

clean:
	rm loader
