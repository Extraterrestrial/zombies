all: 
	gcc -pthread -c main.c -o zombie.o
	gcc -pthread zombie.o -o zombie
