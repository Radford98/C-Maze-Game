all: powelbra.buildrooms powelbra.adventure

powelbra.buildrooms: powelbra.buildrooms.c
	gcc -o powelbra.buildrooms powelbra.buildrooms.c

powelbra.adventure: powelbra.adventure.c
	gcc -o powelbra.adventure powelbra.adventure.c
