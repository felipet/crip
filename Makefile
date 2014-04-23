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
LDLIBS=-lboost_program_options
LDFLAGS=

all: practica1 practica2
.PHONY: all clean mrproper doc practica1 practica2

practica2:  $(BIN)/ejemplos2

practica1:  $(BIN)/ejemplos

generador:  $(BIN)/generador_secuencia

$(BIN)/generador_secuencia: $(OBJ)/generador_secuencia.o
	$(CXX) $(LDLIBS) $(LDFLAGS) $^ -o $@

$(BIN)/ejemplos2: $(OBJ)/libcrip.o $(OBJ)/main2.o
	$(CXX) $(LDLIBS) $(LDFLAGS) $^ -o $@

$(BIN)/ejemplos: $(OBJ)/libcrip.o $(OBJ)/main.o
	$(CXX) $(LDLIBS) $(LDFLAGS) $^ -o $@
	
$(OBJ)/main2.o: $(SRC)/ejemplos_p2.cpp $(SRC)/libcrip.cpp
	$(CXX) $(CXXFLAGS) -I$(INC) $< -o $@

$(OBJ)/main.o: $(SRC)/ejemplos.cpp $(SRC)/libcrip.cpp
	$(CXX) $(CXXFLAGS) -I$(INC) $< -o $@

$(OBJ)/libcrip.o: $(SRC)/libcrip.cpp $(INC)/libcrip.hpp
	$(CXX) $(CXXFLAGS) -I$(INC) $< -o $@
	
$(OBJ)/generador_secuencia.o: $(SRC)/generador_secuencia.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	
clean:
	@rm -f $(OBJ)/*

mrproper: clean
	@echo "Dejando limpito...."
	@rm -f $(BIN)/*
	@rm -rf $(DOC)/*

doc:
	doxygen Doxyfile



