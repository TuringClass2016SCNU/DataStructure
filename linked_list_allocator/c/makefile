main : main.o nallocator.o nmempool.o
	cc -o main main.o nallocator.o nmempool.o \
		-g \
		-Wall 
main.o : main.c nallocator.h nmempool.h
	cc -o main.o -c main.c \
		-g \
		-Wall 
nallocator.o : nallocator.c nallocator.h nmempool.h
	cc -o nallocator.o -c nallocator.c \
		-g \
		-Wall 
nmempool.o : nmempool.c nallocator.h nmempool.h
	cc -o nmempool.o -c nmempool.c \
		-g \
		-Wall 
clean :
	rm *.o
