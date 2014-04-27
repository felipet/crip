#include "libcrip.hpp"      // Mi biblioteca con las funciones criptográficas
#include <chrono>           // Para las medidas de tiempo
#include <iostream>         // Entrada salida
#include <bitset>           // Vector de bits de la STD
#include <string>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

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
}

// Fin archivo ejemplos_p2.cpp
