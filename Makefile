make:
	clear
	rm -f *.o
	rm -f *.a
	rm -rf bin
	as --32 boot.s -o boot.o
