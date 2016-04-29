
ALL:parser

parser: exo4.c
	g++ exo4.c -o exo4 -lfl

exo4.c: exo4.l
	flex -oexo4.c -s -p -p -v exo4.l
