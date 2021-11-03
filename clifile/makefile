all : cli

cli: cli.o cli_init.o downfile.o md5.o up.o
	gcc -o cli cli.o cli_init.o downfile.o md5.o up.o -g

cli.o:
	gcc -c -g cli.c

cli_init.o:
	gcc -c -g cli_init.c

downfile.o:
	gcc -c -g downfile.c

md5.o:
	gcc -c -g md5.c

up.o:
	gcc -c -g up.c

clean:
	rm -f *.o cli
