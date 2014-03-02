################################################################################
##################### Makefile para las prácticas de CRIP ######################
################################################################################

# Ruta de directorios
SRC=./src
INC=./include
OBJ=./obj
BIN=./bin

# Opciones del compilador
CXX=clang++
CXXFLAGS=-std=c++11 -c
LDFLAGS=

all: $(BIN)/ejemplos
.PHONY: all clean mrproper

$(BIN)/ejemplos: $(OBJ)/main.o $(OBJ)/libcrip.o
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ)/main.o: $(SRC)/ejemplos.cpp $(SRC)/libcrip.cpp
	$(CXX) $(CXXFLAGS) -I$(INC) $< -o $@

$(OBJ)/libcrip.o: $(SRC)/libcrip.cpp $(INC)/libcrip.hpp
	$(CXX) $(CXXFLAGS) -I$(INC) $< -o $@
	
clean:
	rm $(OBJ)/*

mrproper: clean
	rm $(BIN)/*




