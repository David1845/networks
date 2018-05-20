all: bbserver bbpeer

bbserver: bbserver.c
	gcc bbserver.c -o bbserver

bbpeer: bbpeer.c
	gcc -pthread -o bbpeer -g bbpeer.c

clean : 
	rm bbclient bbserver

	
