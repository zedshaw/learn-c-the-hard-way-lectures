CFLAGS=-Wall -g

all: logfind
	./logfind || true
	./logfind MAX_LINE
	./logfind error MAX LINE
	./logfind -o error MAX LINE

clean:
	rm -f logfind
