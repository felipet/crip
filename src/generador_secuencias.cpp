/**
 * \file generador_secuencias.cpp
 * \brief Genera un fichero/stream con N secuencias de bits de longitud M a partir
 * de números aleatorios.
 * 
 * \Author Felipe Torres González
 * \date 21-04-2014
 *
 */

#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <stdlib.h>

#define BIT_SIZE 32;

using namespace std;
namespace po = boost::program_options;

long int aleatorio() {
    static long int semilla = 3;
    
    // función aleatoria f(x) = x^2 + 1
    semilla = semilla * semilla + 1;
    
    return semilla;
}

// ------------------------------------------------------------------

int main(int argc, char **argv) {
    bool fichero_salida = false;
    unsigned length, number;
    bitset<32> sec;
    ofstream file;

    
    // Descipción de las opciones del programa
    po::options_description desc("Opciones permitidas");
    desc.add_options()
        ("help,h", "Ayuda del programa")
        ("output,o", po::value<string>(), "Fichero de salida")
        ("number,n", po::value<string>(), "Cuantas secuencias generar")
    ;
    
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm);
    
    if(vm.count("help")) {
        cout << desc << endl;
        return 1;
    }
    
    // Se ha especificado un fichero de salida
    if(!vm["output"].empty()) {
        file.open(vm["output"].as<string>(), ios::trunc);
        fichero_salida = true;
    }
    
    if(!vm["number"].empty()) {
        number = vm["number"].as<unsigned>();
        
        for(unsigned i = 0;i < number;++i) {
            sec = aleatorio();
            file << sec << "\n";
        }
        
        file.close();
        cout << "Archivo generado.\n";
    }
    else {
        cout << "Opciones incorrectas, ver ayuda.\n";
    }
}
