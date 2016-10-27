main : main.o neo_list.o
	cc -o main main.o neo_list.o \
	-g \
	-Wall 
main.o : main.c neo_list.h
	cc -o main.o -c main.c \
	-g \
	-Wall 
neo_list.o : neo_list.c neo_list.h
	cc -o neo_list.o -c neo_list.c \
	-g \
	-Wall 
clean :
	rm *.o
