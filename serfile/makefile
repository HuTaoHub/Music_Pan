all : ser

ser : ser.o thread.o sock_init.o readcnf.o md5sum_check.o getfilename.o downfile.o upfile.o
	gcc -o ser ser.o thread.o -lpthread sock_init.o readcnf.o md5sum_check.o getfilename.o downfile.o upfile.o -g

ser.o :
	gcc -c -g ser.c

thread.o:
	gcc -c -g thread.c

sock_init.o:
	gcc -c -g sock_init.c

readcnf.o:
	gcc -c -g readcnf.c

downfile.o:
	gcc -c -g downfile.c

md5sum_check.o:
	gcc -c -g md5sum_check.c

getfilename.o:
	gcc -c -g getfilename.c

upfile.o:
	gcc -c -g upfile.c
clean:
	rm -f *.o ser
