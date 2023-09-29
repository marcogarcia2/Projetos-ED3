APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

all: libed myapps

libed: 
	gcc -Wall -Werror -Wpedantic -O3 -march=native -c $(SRC)/funcoesFornecidas.c -I $(INCLUDE) -o $(OBJ)/funcoesFornecidas.o

myapps:
	gcc $(APPS)/main.c $(OBJ)/*.o -I $(INCLUDE) -o $(BIN)/main

run:
	$(BIN)/main

test:
	gcc -Wall -Werror -Wpedantic -O3 -march=native $(APPS)/teste.c -o $(BIN)/teste
	$(BIN)/teste
	rm $(BIN)/teste

test2:
	gcc -Wall -Werror -Wpedantic -O3 -march=native $(APPS)/teste2.c -o $(BIN)/teste2
	$(BIN)/teste2

clear:
	del /q obj\* bin\*
	
# gcc -Wall -Werror -Wpedantic -fsanitize=address -O3 -march=native $(APPS)/main.c -o $(BIN)/main
# rm $(BIN)/* $(OBJ)/*