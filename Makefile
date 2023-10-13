APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

all: libed myapps

libed: 
	gcc -Wall -c $(SRC)/funcoesFornecidas.c -I $(INCLUDE) -o $(OBJ)/funcoesFornecidas.o
	gcc -Wall -c $(SRC)/funcoesCriadas.c -I $(INCLUDE) -o $(OBJ)/funcoesCriadas.o
	gcc -Wall -c $(SRC)/funcionalidade1.c -I $(INCLUDE) -o $(OBJ)/funcionalidade1.o
	gcc -Wall -c $(SRC)/funcionalidade2.c -I $(INCLUDE) -o $(OBJ)/funcionalidade2.o
	gcc -Wall -c $(SRC)/funcionalidade3.c -I $(INCLUDE) -o $(OBJ)/funcionalidade3.o
	gcc -Wall -c $(SRC)/funcionalidade4.c -I $(INCLUDE) -o $(OBJ)/funcionalidade4.o
	gcc -Wall -c $(SRC)/lista.c -I $(INCLUDE) -o $(OBJ)/lista.o

myapps:
	gcc $(APPS)/main.c $(OBJ)/*.o -I $(INCLUDE) -o $(BIN)/main

run:
	$(BIN)/main

clear:
	rm $(BIN)/* $(OBJ)/*
	del /q obj\* bin\*
	
# gcc -Wall -Werror -Wpedantic -fsanitize=address -O3 -march=native $(APPS)/main.c -o $(BIN)/main
# rm $(BIN)/* $(OBJ)/*