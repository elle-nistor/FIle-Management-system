build:
	gcc -o search_index main.c file.c tree.c lists.c heap.c
run:
	./search_index
clean:
	rm search_index
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./search_index
pack:
	zip -r Nistor_Daniela_314CB_tema2.zip main.c file.c tree.c lists.c heap.c Makefile README.md search_index.h