AVL_tree: AVL_delete.o AVL_insert.o general_function.o

	clang++ -o AVL_tree: AVL_delete.o AVL_insert.o general_function.o

	AVL_insert.o: AVL_insert.cpp AVL.h
		clang++ -c AVL_insert.cpp

	AVL_delete.o: AVL_delete.cpp AVL.h
		clang++ -c AVL_delete.cpp

	general_function.o: general_function.cpp AVL.h
		clang++ -c general_function.cpp

	clean:
		rm AVL_tree *.o
