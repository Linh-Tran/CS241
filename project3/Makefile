all: huffencode huffdecode

clean:
	rm huffencode huffdecode

huffencode: huffman.h huffman.c huffencode.c
	gcc -o huffencode huffman.c huffencode.c

huffdecode: huffman.h huffman.c huffdecode.c
	gcc -o huffdecode huffman.c huffdecode.c


