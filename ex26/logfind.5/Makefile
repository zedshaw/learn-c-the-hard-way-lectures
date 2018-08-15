CFLAGS=-Wall -g

all: logfind
	./logfind || true
	./logfind MAX_LINE
	./logfind error MAX LINE
	./logfind -o error MAX LINE
	./logfind find me

clean:
	rm -f logfind
