REL_FLAGS = -Wall -Werror -Wpedantic -O3
DEBUG_FLAGS = -Wall -Werror -Wpedantic -Og

C_FILES = main.c fondos.c menu.c dbftool.c ntxtool.c protete.c inputfields.c

all: TETE_R TETE_D

TETE_R: $(C_FILES)
	gcc $(C_FILES) -o TETE -lpdcurses $(REL_FLAGS) -I.

TETE_D: $(C_FILES)
	gcc $(C_FILES) -o TETE_DEBUG -lpdcurses $(DEBUG_FLAGS) -I.

clean:
	rm -f TETE_R TETE_D
