APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src
FLAGS = -Wall -Wpedantic -Werror -march=native -O3 -fsanitize=address
MSG = "Imprime Grafo implementado"

all: libed myapps

libed: 
	@gcc $(FLAGS) -c $(SRC)/funcoesFornecidas.c -I $(INCLUDE) -o $(OBJ)/funcoesFornecidas.o
	@gcc $(FLAGS) -c $(SRC)/funcoesCriadas.c -I $(INCLUDE) -o $(OBJ)/funcoesCriadas.o
	@gcc $(FLAGS) -c $(SRC)/funcoesIndice.c -I $(INCLUDE) -o $(OBJ)/funcoesIndice.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade1.c -I $(INCLUDE) -o $(OBJ)/funcionalidade1.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade2.c -I $(INCLUDE) -o $(OBJ)/funcionalidade2.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade3.c -I $(INCLUDE) -o $(OBJ)/funcionalidade3.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade4.c -I $(INCLUDE) -o $(OBJ)/funcionalidade4.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade5.c -I $(INCLUDE) -o $(OBJ)/funcionalidade5.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade6.c -I $(INCLUDE) -o $(OBJ)/funcionalidade6.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade7.c -I $(INCLUDE) -o $(OBJ)/funcionalidade7.o
	@gcc $(FLAGS) -c $(SRC)/funcionalidade8.c -I $(INCLUDE) -o $(OBJ)/funcionalidade8.o
	@gcc $(FLAGS) -c $(SRC)/lista.c -I $(INCLUDE) -o $(OBJ)/lista.o
	@gcc $(FLAGS) -c $(SRC)/grafo.c -I $(INCLUDE) -o $(OBJ)/grafo.o

myapps:
	@gcc $(FLAGS) $(APPS)/main.c $(OBJ)/*.o -I $(INCLUDE) -o $(BIN)/main

commit:
	git pull origin main
	git add .
	git commit -m $(MSG)
	git push origin main

run:
	$(BIN)/main

clean:
	rm $(BIN)/* $(OBJ)/*