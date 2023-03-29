traceroute: Makefile traceroute.o sender.o reciver.o
	gcc -std=gnu17 -W -Wall -o traceroute traceroute.o sender.o reciver.o

traceroute.o:
	gcc -std=gnu17 -W -Wall -c traceroute.c -o traceroute.o

sender.o:
	gcc -std=gnu17 -W -Wall -c sender.c -o sender.o

reiver.0:
	gcc -std=gnu17 -W -Wall -c reciver.c -o reciver.o

two: Makefile traceroute.o sender.o reciver.o
	gcc -std=gnu17 -W -Wall -o traceroute1 traceroute.o sender.o reciver.o
	gcc -std=gnu17 -W -Wall -o traceroute2 traceroute.o sender.o reciver.o

clean:
	rm -f *.o

distclean:
	rm -f *.o rm -f traceroute traceroute1 traceroute2