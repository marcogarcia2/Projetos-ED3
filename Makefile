APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

all: libed myapps

libed: 
	gcc -Wall -Werror -Wpedantic -fsanitize=address -O3 -march=native -c $(SRC)/funcoesFornecidas.c -I $(INCLUDE) -o $(OBJ)/funcoesFornecidas.o

myapps:
	gcc -Wall -Werror -Wpedantic -fsanitize=address -O3 -march=native $(APPS)/main.c $(OBJ)/*.o -I $(INCLUDE) -o $(BIN)/main

run:
	$(BIN)/main

clear: