CFLAGS=-g -O0 -std=c99 -Wall -Werror -Wconversion -Wtype-limits -pedantic

minipruebas: abb_minipruebas.c abb.c
	gcc abb_minipruebas.c abb.c $(FLAGS) -o minipruebas

ejecutar_minipruebas: minipruebas
	clear
	./minipruebas

valgrind_minipruebas: minipruebas
	clear
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./minipruebas

debug_minipruebas: minipruebas
	clear
	gdb ./minipruebas



testing: testing.c abb.c
	gcc testing.c abb.c $(FLAGS) -o testing

ejecutar_testing: testing
	clear
	./testing

valgrind_testing: testing
	clear
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./testing

debug_testing: testing
	clear
	gdb ./testing