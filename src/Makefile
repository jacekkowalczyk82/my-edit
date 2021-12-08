.PHONY: outdir

all: myedit

outdir: 
	mkdir -p ../bin/

myedit: outdir myedit.c
	$(CC) myedit.c -o ../bin/myedit -Wall -Wextra -pedantic -std=c17


