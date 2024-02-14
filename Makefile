build:my_octave
my_octave:my_octave.c
	gcc  -Wall -Wextra my_octave.c -o my_octave
pack:
	zip -FSr 314CA_CiulincaAndraStefania_Tema2.zip README Makefile *.c *.h
clean:
	rm -f my_octave

.PHONY: pack clean
