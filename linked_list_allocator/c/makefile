main : main.o nallocator.o
	cc -o main main.o nallocator.o \
	-I. \
	-g \
	-Wall 
main.o : main.c nallocator.h
	cc -o main.o -c main.c \
	-I. \
	-g \
	-Wall 
nallocator.o : nallocator.c nallocator.h
	cc -o nallocator.o -c nallocator.c \
	-I. \
	-g \
	-Wall 
clean :
	rm *.o
