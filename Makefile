INCLUDE = ./include
SRC = ./src
BIN = ./bin
OBJ = ./obj

CXX = g++ -std=c++11
SFML = -lsfml-graphics -lsfml-window -lsfml-system

OBJS =  $(OBJ)/main.o \
		$(OBJ)/Bus.o \
		$(OBJ)/Cpu.o \
		$(OBJ)/Memory.o 

all: $(OBJS)
	$(CXX) -o $(BIN)/emulator $(OBJS) -I $(INCLUDE) $(SFML)
	@$(BIN)/emulator

$(OBJ)/main.o: main.cpp
	$(CXX) -c main.cpp -I $(INCLUDE) -o $(OBJ)/main.o

$(OBJ)/Bus.o: $(SRC)/Bus.cpp
	$(CXX) -c $(SRC)/Bus.cpp -I $(INCLUDE) -o $(OBJ)/Bus.o

$(OBJ)/Cpu.o: $(SRC)/Cpu.cpp
	$(CXX) -c $(SRC)/Cpu.cpp -I $(INCLUDE) -o $(OBJ)/Cpu.o

$(OBJ)/Memory.o: $(SRC)/Memory.cpp
	$(CXX) -c $(SRC)/Memory.cpp -I $(INCLUDE) -o $(OBJ)/Memory.o

clean:
	@rm -rf obj
	@mkdir obj
	
run:
	@$(BIN)/emulator
