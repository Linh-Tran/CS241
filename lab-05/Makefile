all: listtest treetest

clean:
	rm listtest treetest

listtest: linkedlist.h linkedlist.c listtest.c
	gcc -o listtest linkedlist.c listtest.c

treetest: binarytree.h binarytree.c treetest.c
	gcc -o treetest binarytree.c treetest.c

