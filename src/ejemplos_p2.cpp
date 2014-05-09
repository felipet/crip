#include "libcrip.hpp"      // Mi biblioteca con las funciones criptográficas
#include <chrono>           // Para las medidas de tiempo
#include <iostream>         // Entrada salida
#include <bitset>           // Vector de bits de la STD
#include <string>
#include <sstream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <fstream>

using namespace std;
namespace ch = std::chrono;
namespace po = boost::program_options;

int main(int argc, char **argv) {
    bool tiempos = false;
    ch::high_resolution_clock::time_point t1;
    
    // Descipción de las opciones del programa
    po::options_description desc("Opciones permitidas");
    desc.add_options()
        ("help,h", "Ayuda del programa")
        ("p1,1", po::value<string>(), "Primer postulado de Golomb")
        ("p2,2", po::value<string>(), "Segundo postulado de Golomb")
        ("p3,3", po::value<string>(), "Tercer postulado de Golomb")
        ("golomb,g", po::value<string>(), "Comprobar postulados de Golomb")
        ("lfsr,r", po::value<vector<string> >()->multitoken(), "LFSR")
        ("nlfsr,n", po::value<vector<string> >()->multitoken(), "NLFSR")
        ("berlekamp_massey,b", po::value<string>(), "Algoritmo Berlekamp-Massey")
        ("cifrar,c", po::value<vector<string> >()->multitoken(), "Cifrado en flujo usando generador de Geffe")
        ("tiempos,t", po::value<string>()->implicit_value("0"), "Devuelve tiempos de ejecución")
    ;
    
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm);
    
    if(vm.count("help")) {
        cout << desc << endl;
        return 1;
    }
    
    tiempos = vm.count("tiempos") ? true : false;
    
    // Primer postulado de Golomb
    
    if(!vm["p1"].empty()) {
        string cadena(vm["p1"].as<string>());
        bitset<BIT_SIZE> sec(cadena);
        unsigned size = cadena.size();
        
        if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
                
        bool cumple = primer_postulado_Golomb(sec, size);
        
        auto t2 = ch::high_resolution_clock::now();
        
        cout << "\tLa cadena : " << cadena << endl;
        cout <<"\t¿Cumple el primer postulado de Golomb? : ";
        cout << (cumple? "Si" : "No") << endl;
        
        if(tiempos)
            cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
            (t2 - t1).count() << "us" << endl;
        
        return 0;
    }
    
    // Segundo postulado de Golomb
    
    if(!vm["p2"].empty()) {
        string cadena(vm["p2"].as<string>());
        bitset<BIT_SIZE> sec(cadena);
        unsigned size = cadena.size();
        
        if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
                
        bool cumple = segundo_postulado_Golomb(sec, size);
        
        auto t2 = ch::high_resolution_clock::now();
        
        cout << "\tLa cadena : " << cadena << endl;
        cout <<"\t¿Cumple el segundo postulado de Golomb? : ";
        cout << (cumple? "Si" : "No") << endl;
        
        if(tiempos)
            cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
            (t2 - t1).count() << "us" << endl;
        
        return 0;
    }
    
    // Tercer postulado de Golomb
    
    if(!vm["p3"].empty()) {
        string cadena(vm["p3"].as<string>());
        bitset<BIT_SIZE> sec(cadena);
        unsigned size = cadena.size();
        
        if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
                
        bool cumple = tercer_postulado_Golomb(sec, size);
        
        auto t2 = ch::high_resolution_clock::now();
        
        cout << "\tLa cadena : " << cadena << endl;
        cout <<"\t¿Cumple el tercer postulado de Golomb? : ";
        cout << (cumple? "Si" : "No") << endl;
        
        if(tiempos)
            cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
            (t2 - t1).count() << "us" << endl;
        
        return 0;
    }
    
    // Comprobar postulados de Golomb
    
    if(!vm["golomb"].empty()) {
        string cadena(vm["golomb"].as<string>());
        bitset<BIT_SIZE> sec(cadena);
        unsigned size = cadena.size();
        
        if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
                
        bool cumple = cumple_postulados_Golomb(sec, size);
        
        auto t2 = ch::high_resolution_clock::now();
        
        cout << "\tLa cadena : " << cadena << endl;
        cout <<"\t¿Cumple los postulados de Golomb? : ";
        cout << (cumple? "Si" : "No") << endl;
        
        if(tiempos)
            cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
            (t2 - t1).count() << "us" << endl;
        
        return 0;
    }
    
    // Ejercicio2 : LFSR
    
    if(!vm["lfsr"].empty()) {
        vector<string> opc = vm["lfsr"].as<vector<string> >();
        
        if(opc.size() < 3) {
            cout << "\t Número de parámetros incorrectos\n";
            return 0;
        }
    
        bitset<BIT_SIZE> coefs(opc[0]);
        unsigned size_coefs = opc[0].size();
        bitset<BIT_SIZE> seed(opc[1]);
        unsigned size_seed = opc[1].size();
        bitset<BIT_SIZE> out;
        unsigned size_out;
        istringstream (opc[2]) >> size_out;
        
        if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
                
        int error = LFSR(coefs, size_coefs, seed, size_seed, out, size_out);
        
        auto t2 = ch::high_resolution_clock::now();
        
        cout << "\tLa cadena : " << out << endl;
        
        if(tiempos)
            cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
            (t2 - t1).count() << "us" << endl;
        
        return 0;
    }
    
    // Ejercicio3 : NLFSR
    
    if(!vm["nlfsr"].empty()) {
        vector<string> opc = vm["nlfsr"].as<vector<string> >();
        
        if(opc.size() < 4) {
            cout << "\t Número de parámetros incorrectos\n";
            return 0;
        }
    
        bitset<BIT_SIZE> lista_monomios(opc[0]);
        unsigned size_lista = opc[0].size();
        unsigned num_var;
        istringstream (opc[1]) >> num_var;
        bitset<BIT_SIZE> seed(opc[2]);
        unsigned size_seed = opc[1].size();
        unsigned k;
        istringstream (opc[3]) >> k;
        bitset<BIT_SIZE> sec_out;
        
        if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
                
        int error = NLFSR(lista_monomios, size_lista, num_var, seed, k, sec_out);
        
        auto t2 = ch::high_resolution_clock::now();
        
        cout << "\tCadena generada: " << sec_out << endl;
        
        if(tiempos)
            cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
            (t2 - t1).count() << "us" << endl;
        
        return 0;
    }
    
    // Cifrado en flujo de un fichero usando generador de Geffe
    // Ejercicio2 : LFSR
    
    if(!vm["cifrar"].empty()) {
        
        vector<string> opc = vm["cifrar"].as<vector<string> >();
        
        if(opc.size() < 2) {
            cout << "\t Número de parámetros incorrectos\n";
            return 0;
        }
    
        // TODO: Comprobaciones relacionadas con ficheros
        
        if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
                
        int error = cifrado_flujo(opc[1], opc[2]);
        
        auto t2 = ch::high_resolution_clock::now();
        
        cout << "\tFichero (des)cifrado " << error << endl;
        
        if(tiempos)
            cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
            (t2 - t1).count() << "us" << endl;
        
        return 0;
    }
    
    // Algoritmo Berlekamp Massey
    if(!vm["berlekamp_massey"].empty()) {
        string opc = vm["berlekamp_massey"].as<string>();
        unsigned complejidad;
        bitset<BIT_SIZE> sec(opc);
        
        if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
                
        int error = berlekamp_massey(sec, opc.size(), complejidad);
        
        auto t2 = ch::high_resolution_clock::now();
        
        cout << "\tLa cadena : " << sec << endl;
        cout << "\tComplejidad : " << complejidad << endl;
        
        if(tiempos)
            cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
            (t2 - t1).count() << "us" << endl;
        
        return 0;
    }
}

// Fin archivo ejemplos_p2.cpp
