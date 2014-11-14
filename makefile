all: a.out 

a.out: main.c
	cc -g main.c -o a.out


clean:
	rm a.out
