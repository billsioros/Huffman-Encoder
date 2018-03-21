
CC     = g++
CFLAGS = -W -o3

PATH_SRC = ./src/
PATH_BIN = ./bin/

MAIN_DEP = $(addprefix $(PATH_SRC), Utility.h main.cpp)
UTIL_DEP = $(addprefix $(PATH_SRC), Huffman.h Utility.h Utility.cpp)
HUFF_DEP = $(addprefix $(PATH_SRC), Huffman.h Huffman.cpp)
OBJS     = $(addprefix $(PATH_BIN), main.o Utility.o Huffman.o)

$(PATH_BIN)huffencoder : $(OBJS)
	@echo Compiling executable file 'huffencoder'
	$(CC) $(CFLAGS) $(OBJS) -o $(PATH_BIN)huffencoder

$(PATH_BIN)main.o : $(MAIN_DEP)
	@echo Compiling object file 'main.o'
	$(CC) $(CFLAGS) $(PATH_SRC)main.cpp -c -o $(PATH_BIN)main.o

$(PATH_BIN)Utility.o : $(UTIL_DEP)
	@echo Compiling object file 'Utility.o'
	$(CC) $(CFLAGS) $(PATH_SRC)Utility.cpp -c -o $(PATH_BIN)Utility.o

$(PATH_BIN)Huffman.o : $(HUFF_DEP)
	@echo Compiling object file 'Huffman.o'
	$(CC) $(CFLAGS) $(PATH_SRC)Huffman.cpp -c -o $(PATH_BIN)Huffman.o

.PHONY clean:
	rm -i $(PATH_BIN)*