APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

all: libed myapps

libed: 
	gcc -Wall -Wpedantic -Werror -march=native -O3 -fsanitize=address -c $(SRC)/funcoesFornecidas.c -I $(INCLUDE) -o $(OBJ)/funcoesFornecidas.o
	gcc -Wall -Wpedantic -Werror -march=native -O3 -fsanitize=address -c $(SRC)/funcoesCriadas.c -I $(INCLUDE) -o $(OBJ)/funcoesCriadas.o
	gcc -Wall -Wpedantic -Werror -march=native -O3 -fsanitize=address -c $(SRC)/funcionalidade1.c -I $(INCLUDE) -o $(OBJ)/funcionalidade1.o
	gcc -Wall -Wpedantic -Werror -march=native -O3 -fsanitize=address -c $(SRC)/funcionalidade2.c -I $(INCLUDE) -o $(OBJ)/funcionalidade2.o
	gcc -Wall -Wpedantic -Werror -march=native -O3 -fsanitize=address -c $(SRC)/funcionalidade3.c -I $(INCLUDE) -o $(OBJ)/funcionalidade3.o
	gcc -Wall -Wpedantic -Werror -march=native -O3 -fsanitize=address -c $(SRC)/funcionalidade4.c -I $(INCLUDE) -o $(OBJ)/funcionalidade4.o
	gcc -Wall -Wpedantic -Werror -march=native -O3 -fsanitize=address -c $(SRC)/lista.c -I $(INCLUDE) -o $(OBJ)/lista.o

myapps:
	gcc -Wall -Wpedantic -Werror -march=native -O3 -fsanitize=address $(APPS)/main.c $(OBJ)/*.o -I $(INCLUDE) -o $(BIN)/main

run:
	$(BIN)/main

clean:
	rm $(BIN)/* $(OBJ)/*