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
        ("mcd_ext,M", po::value<vector<string> >()->multitoken(), "Calcula el máximo común divisor de varios números (algoritmo extendido)")
        ("mcd,m", po::value<vector<string> >()->multitoken(), "Calcula el máximo común divisor de varios números")
        ("inverso,i", po::value<vector<string> >()->multitoken(),"Calcula el inverso de un número modulo n")
        ("pot,x",po::value<vector<string> >()->multitoken(), "Calcula el valor de a^m mod n")
        ("primo,p", po::value<string>(), "Test de primalidad Miller-Rabin")
        ("log,l", po::value<vector<string> >()->multitoken(), "Calcula el logaritmo discreto para a^b = c mod p")
        ("fermat,f", po::value<string>(), "Método de factorización de Fermat")
        ("pollard,r", po::value<string>(), "Método de factorización rho-Pollard")
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
    
    // Comprobación de las opciones
    
    // Opción para el mcd con el algoritmo extendido de Euclides
    if(!vm["mcd_ext"].empty()) {
        opc = vm["mcd_ext"].as<vector<string> >();
        size = opc.size();
        
        if(size > 1) {
            INT_TYPE mcd, a , b;
            
            values = new INT_TYPE*[size];
            
            for(auto x : opc) {
                values[i] = new INT_TYPE(x.c_str());
                ++i;
            }
            
            a = *values[0];
            b = *values[1];
            
            if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
            mcd = mcd_ext(*values[0], *values[1]);
            auto t2 = ch::high_resolution_clock::now();
            
            cout << "\tmcd_ext: " << mcd << " = ";
            cout << "\t(" << a << " * " << *values[0];
            cout << ") +\n\t\t\t(" << b << " * " << *values[1] << ")" << endl; 
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
    
    // Opción para mcd con el algoritmo de Euclides
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
            
            if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
            mcd = MCD(*values[0], *values[1]);
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
    
    // Opción para el inverso
    
    if(!vm["inverso"].empty()) {
        bool p;
        INT_TYPE res;
        opc = vm["inverso"].as<vector<string> >();
        size = opc.size();
    
        if(size > 1) {
            size = opc.size();
            INT_TYPE log;
            
            values = new INT_TYPE*[size];
            
            for(auto x : opc) {
               values[i] = new INT_TYPE(x.c_str());
                ++i;
            }
            
            if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
            p = inverso(*values[0], *values[1], res);
            auto t2 = ch::high_resolution_clock::now();
            
            if(p) {
                cout << "\tInverso: \n\t";
                cout << "\t" << res << " = " << *values[0] << "^(-1) mod " <<
                *values[1] << endl;
            }
            else cout << "No se puede calcular el inverso\n";
            
            if(tiempos)
                cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
                (t2 - t1).count() << "us" << endl;
            
            return 0;
        }
    }
    
    // Opción para el logaritmo discreto
    
    if(!vm["log"].empty()) {
        opc = vm["log"].as<vector<string> >();
        size = opc.size();
    
        if(size > 1) {
            size = opc.size();
            INT_TYPE log;
            
            values = new INT_TYPE*[size];
            
            for(auto x : opc) {
               values[i] = new INT_TYPE(x.c_str());
                ++i;
            }

            if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
            log = log_discreto(*values[0], *values[1], *values[2]);
            auto t2 = ch::high_resolution_clock::now();
            
            cout << "\tLogaritmo discreto:\t" << log << endl;
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
    
    if(!vm["fermat"].empty()) {
        bool p = false;
        INT_TYPE x,y;
        string opc = vm["fermat"].as<string>();
        
        if(opc.size() > 0) {
            INT_TYPE value(opc.c_str());
            
            if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
            p = metodo_fermat(value, x, y);
            auto t2 = ch::high_resolution_clock::now();
            
            if(p) {
                cout << "\tFactorización de Fermat: \n\t";
                cout << value << " = (" << x << " + " << y << ") (";
                cout << x << " - " << y << ")\n";
            }
            else cout << "No se ha podido factorizar\n";
            
            if(tiempos)
                cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
                (t2 - t1).count() << "us" << endl;
            
            return 0;
        }
    }
    
    if(!vm["pollard"].empty()) {
        INT_TYPE f, f2;
        string opc = vm["pollard"].as<string>();
        
        if(opc.size() > 0) {
            INT_TYPE value(opc.c_str());
            
            if(tiempos)
                t1 = ch::high_resolution_clock::now(); 
            f = metodo_rho_pollard(value);
            auto t2 = ch::high_resolution_clock::now();
            
            cout << "\tFactorización rho-Pollard: \n\t";
            if(f > 1) {
                f2 = value / f;
                cout << value << " = (" << f << " * " << f2 << ")\n";
            }
            else{
                if(f == 1) cout << value << " probablemente primo\n";
                else cout << "No se ha podido factorizar\n";
            }
            
            if(tiempos)
                cout << "\tTiempo:\t" << chrono::duration_cast<chrono::microseconds >
                (t2 - t1).count() << "us" << endl;
            
            return 0;
        }
    }
    
    cout << desc << endl;
    
    return 0;
}
