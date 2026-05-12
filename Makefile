build:
	gcc -o search_index main.c file.c tree.c lists.c heap.c
run:
	./search_index
clean:
	rm search_index
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./search_index