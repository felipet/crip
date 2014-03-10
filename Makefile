################################################################################
##################### Makefile para las prácticas de CRIP ######################
################################################################################

# Ruta de directorios
SRC=./src
INC=./include
OBJ=./obj
BIN=./bin
DOC=./doc

# Opciones del compilador
CXX=clang++
CXXFLAGS=-std=c++11 -c
LDFLAGS=-lboost_program_options

all: $(BIN)/ejemplos
.PHONY: all clean mrproper doc

$(BIN)/ejemplos: $(OBJ)/libcrip.o $(OBJ)/main.o
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ)/main.o: $(SRC)/ejemplos.cpp $(SRC)/libcrip.cpp
	$(CXX) $(CXXFLAGS) -I$(INC) $< -o $@

$(OBJ)/libcrip.o: $(SRC)/libcrip.cpp $(INC)/libcrip.hpp
	$(CXX) $(CXXFLAGS) -I$(INC) $< -o $@
	
clean:
	@rm -f $(OBJ)/*

mrproper: clean
	@echo "Dejando limpito...."
	@rm -f $(BIN)/*
	@rm -rf $(DOC)/*

doc:
	doxygen Doxyfile



