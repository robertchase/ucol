all :	ucol
ucol :	ucol.c
	gcc ucol.c -o ucol
install : ucol
	mv ucol $(HOME)/bin
