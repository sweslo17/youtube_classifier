CC = gcc
FLAG = -lm -o
OPT = -O2
TARGET= classify
DEBUG= -g
SOURCE= gais_util.c youtube_util.c classify.c
all:
	$(CC) $(OPT) $(FLAG) $(TARGET) $(SOURCE)
warning:
	gcc -ansi -pedantic -Wall -O2 -lm -g -o classify classify.c
debug:
	gcc -DDBUG -o classify classify.c
gdb:
	$(CC) $(DEBUG) $(FLAG) $(TARGET) $(SOURCE)
	gdb --args classify class_trans category.map test_input
gdb-test:
	$(CC) $(DEBUG) $(FLAG) $(TARGET) $(SOURCE)
	gdb --args classify class_trans category.map test_input2
clean:
	rm -f *.core
