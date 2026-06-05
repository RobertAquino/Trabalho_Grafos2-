# Nome do executável final
MAIN := main

# Lista de objetos (arquivos .o que serão gerados a partir dos .cpp)
# Como PuzzleParser e Estruturas são apenas arquivos .hpp (header-only), 
# eles não geram arquivos .o separados. Temos apenas o main.o por enquanto.
OBJECTS := main.o MotorBusca.o

# Compilador e Flags
CC := g++
# -Wall -Wextra: Ativa avisos de possíveis erros no código
# -std=c++17: Padrão moderno do C++
# -g: Permite depuração (útil para encontrar erros de memória)
# -IBibliotecas: Permite usar #include "Estruturas.hpp" sem digitar o caminho completo no main
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
# Ele depende do main.cpp e dos headers incluídos na pasta Bibliotecas
main.o: main.cpp Bibliotecas/Estruturas.hpp Bibliotecas/PuzzleParser.hpp Bibliotecas/MotorBusca.hpp
	$(CC) $(FLAGS) -c main.cpp -o main.o


MotorBusca.o: Src/MotorBusca.cpp Bibliotecas/MotorBusca.hpp Bibliotecas/Estruturas.hpp
	$(CC) $(FLAGS) -c Src/MotorBusca.cpp -o MotorBusca.o

# Limpeza de arquivos temporários (Adaptada para funcionar em Windows e Linux)
clean:
	-$(RM) main.o MotorBusca.o
	-$(RM) $(OUTPUTMAIN)
	@echo Limpeza de arquivos .o e executaveis completa!

# Atalho para compilar e executar
run: all
	$(EXEC)