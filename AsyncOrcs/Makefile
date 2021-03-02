assignment4:	main.o fnc.o go.o graphics.o
	gcc -o assignment4 main.o fnc.o go.o graphics.o -lpthread -lncurses

main.o: main.c defs.h
	gcc -c main.c
	
fnc.o: fnc.c defs.h
	gcc -c fnc.c

go.o: go.c defs.h
	gcc -c go.c
	
graphics.o: graphics.c defs.h
	gcc -c graphics.c
	
clean:
	rm -f *.o
