# Nome do executável final
MAIN := main

# Lista de objetos (arquivos .o que serão gerados a partir dos .cpp)
OBJECTS := main.o MotorBusca.o Heuristicas.o

# Compilador e Flags
CC := g++
FLAGS := -Wall -Wextra -std=c++17 -O3 -IBibliotecas

# Identificação do Sistema Operacional para comandos de terminal
ifeq ($(OS), Windows_NT)
	OUTPUTMAIN := $(MAIN).exe
	RM := del /q /f
	EXEC := $(OUTPUTMAIN)
else
	OUTPUTMAIN := $(MAIN).out
	RM := rm -f
	EXEC := ./$(OUTPUTMAIN)
endif

# Alvo principal: Compila tudo
all: $(OUTPUTMAIN)
	@echo Compilacao completa com sucesso!

# Linkagem: Une os objetos para criar o executável
$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN)

# Regra para compilar o main.o
main.o: main.cpp Bibliotecas/Estruturas.hpp Bibliotecas/PuzzleParser.hpp Bibliotecas/MotorBusca.hpp Bibliotecas/Heuristicas.hpp
	$(CC) $(FLAGS) -c main.cpp -o main.o

# Regra para compilar o MotorBusca.o
MotorBusca.o: Src/MotorBusca.cpp Bibliotecas/MotorBusca.hpp Bibliotecas/Estruturas.hpp Bibliotecas/Heuristicas.hpp
	$(CC) $(FLAGS) -c Src/MotorBusca.cpp -o MotorBusca.o

# Regra para compilar o Heuristicas.o
Heuristicas.o: Src/Heuristicas.cpp Bibliotecas/Heuristicas.hpp
	$(CC) $(FLAGS) -c Src/Heuristicas.cpp -o Heuristicas.o

# Limpeza de arquivos temporários
clean:
	-$(RM) main.o MotorBusca.o Heuristicas.o
	-$(RM) $(OUTPUTMAIN)
	@echo Limpeza de arquivos .o e executaveis completa!

# Atalho para compilar e executar
run: all
	$(EXEC)