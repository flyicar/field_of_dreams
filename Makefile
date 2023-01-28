.PHONY: all clean

all: hserv 

hserv.o: hserv.c
	gcc -c -o hserv.o hserv.c
serv.o: serv.c
	gcc -c -o serv.o serv.c
prot.o: prot.c
	gcc -c -o prot.o prot.c

hserv: hserv.o serv.o prot.o
	gcc -o hserv hserv.o serv.o prot.o

clean:
	rm -rf *.o hserv
