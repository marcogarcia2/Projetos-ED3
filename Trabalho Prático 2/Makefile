APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src
FLAGS = -Wall -Wpedantic -Werror #-march=native -O3 -fsanitize=address
MSG = "Funcionalidade 10 funcionando nos casos do runcodes"

all: libed myapps

libed: 
	@gcc $(FLAGS) -c $(SRC)/funcoesFornecidas.c -I $(INCLUDE) -o $(OBJ)/funcoesFornecidas.o
	@gcc $(FLAGS) -c $(SRC)/funcoesCriadas.c -I $(INCLUDE) -o $(OBJ)/funcoesCriadas.o
	@gcc $(FLAGS) -c $(SRC)/geradorGrafo.c -I $(INCLUDE) -o $(OBJ)/geradorGrafo.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade8.c -I $(INCLUDE) -o $(OBJ)/funcionalidade8.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade9.c -I $(INCLUDE) -o $(OBJ)/funcionalidade9.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade10.c -I $(INCLUDE) -o $(OBJ)/funcionalidade10.o
	@gcc $(FLAGS) -c $(SRC)/grafo.c -I $(INCLUDE) -o $(OBJ)/grafo.o

myapps:
	@gcc $(FLAGS) $(APPS)/main.c $(OBJ)/*.o -I $(INCLUDE) -o $(BIN)/main

commit:
	git pull origin main
	git add .
	git commit -m $(MSG)
	git push origin main

run:
	@$(BIN)/main

clean:
	rm $(BIN)/* $(OBJ)/*