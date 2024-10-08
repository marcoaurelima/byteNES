INCLUDE = ./include
SRC = ./src
BIN = ./bin
OBJ = ./obj

CXX = clang++ -O3 -flto -std=c++11 -Wall -Wextra -Wpedantic -Werror
SFML = -lsfml-graphics -lsfml-window -lsfml-system

OBJS =  $(OBJ)/main.o \
		$(OBJ)/Cpu.o \
		$(OBJ)/Mem.o \
		$(OBJ)/Gui.o 
		

all: $(OBJS)
	$(CXX) -o $(BIN)/emulator $(OBJS) -I $(INCLUDE) $(SFML)
	@$(BIN)/emulator

$(OBJ)/main.o: main.cpp
	$(CXX) -c main.cpp -I $(INCLUDE) -o $(OBJ)/main.o

$(OBJ)/Cpu.o: $(SRC)/Cpu.cpp
	$(CXX) -c $(SRC)/Cpu.cpp -I $(INCLUDE) -o $(OBJ)/Cpu.o

$(OBJ)/Mem.o: $(SRC)/Mem.cpp
	$(CXX) -c $(SRC)/Mem.cpp -I $(INCLUDE) -o $(OBJ)/Mem.o

$(OBJ)/Gui.o: $(SRC)/Gui.cpp
	$(CXX) -c $(SRC)/Gui.cpp -I $(INCLUDE) -o $(OBJ)/Gui.o


clean:
	@rm -rf obj
	@rm -rf bin
	@mkdir obj
	@mkdir bin
	
run:
	@$(BIN)/emulator

assembly:
	asm6f asm/program.asm asm/program.bin
