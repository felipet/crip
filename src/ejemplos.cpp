#include "libcrip.hpp"      // Mi biblioteca con las funciones criptográficas
#include <chrono>           // Para las medidas de tiempo
#include <iostream>         // Entrada salida
#include <vector>           // Vector de la STD
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

using namespace std;
namespace ch = std::chrono;
namespace po = boost::program_options;

int main(int argc, char **argv) {
    bool tiempos = false;
    ch::high_resolution_clock::time_point t1;
    INT_TYPE **values = 0;
    unsigned i = 0, size;
    vector<string> opc;
    
    // Descipción de las opciones del programa
    po::options_description desc("Opciones permitidas");
    desc.add_options()
        ("help,h", "Ayuda del programa")
        ("mcd,m", po::value<vector<string> >()->multitoken(), "Calcula el máximo común divisor de varios números")
        ("inverso,i", po::value<vector<string> >()->multitoken(),"Calcula el inverso de un número modulo n")
        ("pot,x",po::value<vector<string> >()->multitoken(), "Calcula el valor de a^m mod n")
        ("primo,p", po::value<string>(), "Test de primalidad Miller-Rabin")
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
    
    if(!vm["mcd"].empty()) {
        opc = vm["mcd"].as<vector<string> >();
        size = opc.size();
        
        if(size > 1) {
            INT_TYPE mcd;
            
            values = new INT_TYPE*[size];
            
            for(auto x : opc) {
                values[i] = new INT_TYPE(x.c_str());
                ++i;
            }
            
            mcd = *values[0];
            if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
            for(i = 1;i < size; i++)
                mcd = mcd_ext(mcd, *values[i]);
                // No se guardan u y v, cambiar si hace falta más tarde
            auto t2 = ch::high_resolution_clock::now();
            
            cout << "\tmcd:\t" << mcd << endl;
            if(tiempos)
                cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
                (t2 - t1).count() << "us" << endl;
            
            for(i = 0;i < size;i++)
                delete[] values[i];
            delete[] values;
            
            return 0;
        }
        else {
            cout << "\tNúmero de parámetros incorrectos" << endl;
            return 1;
        }
    }
    
    
    if(!vm["pot"].empty()) {
        opc = vm["pot"].as<vector<string> >();
        size = opc.size();
    
        if(size > 1) {
            size = opc.size();
            INT_TYPE pot;
            
            values = new INT_TYPE*[size];
            
            for(auto x : opc) {
               values[i] = new INT_TYPE(x.c_str());
                ++i;
            }
            
            cout << "ekekke" << endl;
            
            if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
            pot = potencia_mod(*values[0], *values[1], *values[2]);
            auto t2 = ch::high_resolution_clock::now();
            
            cout << "\tpotencia modular:\t" << pot << endl;
            if(tiempos)
                cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
                (t2 - t1).count() << "us" << endl;
            
            for(i = 0;i < size;i++)
                delete[] values[i];
            delete[] values;
            
            return 0; 
        }
        else {
            cout << "\tNúmero de parámetros incorrectos" << endl;
            return 1;
        }
    }
    
    if(!vm["primo"].empty()) {
        bool p = false;
        string opc = vm["primo"].as<string>();
        
        if(opc.size() > 0) {
            INT_TYPE value(opc.c_str());
            
            if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
            p = es_primo(value);
            auto t2 = ch::high_resolution_clock::now();
            
            cout << "\t" << opc << " ";
            if(p) cout << "primo\n";
            else cout << "no es primo\n";
            
            if(tiempos)
                cout << "\tTiempo:\t" << chrono::duration_cast<chrono::milliseconds >
                (t2 - t1).count() << "ms" << endl;
            
            return 0;
        }
    }
    
    
    return 0;
}
