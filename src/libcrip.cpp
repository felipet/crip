#include "libcrip.hpp"
#include <random>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <map>


//#DEFINE DEBUG

INT_TYPE mcd(INT_TYPE a, INT_TYPE b) {
    INT_TYPE r;
    
    while(!b) {
        r = a % b;
        a = b;
        b = r;
    }
    
    return a;
}

// ------------------------------------------------------------------

INT_TYPE mcd_ext(INT_TYPE &a, INT_TYPE &b) {
    INT_TYPE x[3], y[3], q, r;
    int i = 1;
    bool cambiados = false;
     
     // Caso base
     if(!b) {
        a = 1;
        b = 0;
        
        return a;
     }
     
     // Cambiar el orden para ahorrar pasos
     if(a < b) {
        a ^= b;
        b ^= a;
        a ^= b;
        cambiados = true;
     }
     
     x[2] = y[1] = 1;
     x[1] = y[2] = 0;
     
     while(b > 0) {
        q    = a / b; // División entera
        r    = a - q * b;
        x[0] = x[2] - q * x[1];
        y[0] = y[2] - q * y[1];
        a    = b;
        b    = r;
        x[2] = x[1];
        x[1] = x[0];
        y[2] = y[1];
        y[1] = y[0];
        
        #ifdef DEBUG
        using namespace std;
            cout << "Iteración : " << i << endl;
            cout << "q" << "\t" << "r" << "\t" << "x" << "\t" << "y" << "\t";
            cout << "a" << "\t" << "b" << "\t" << "x2" << "\t" << "x1" << "\t";
            cout << "y2" << "\t" << "y1" << "\t" << endl;
            cout << q << "\t" << r << "\t" << x[0] << "\t" << y[0] << "\t";
            cout << a << "\t" << b << "\t" << x[2] << "\t" << x[1] << "\t";
            cout << y[2] << "\t" << y[1] << "\t" << endl;
        using namespace boost::multiprecision;
        ++i;
        #endif
     }
     
     // Salida del algoritmo
     r = a;
     if(!cambiados) {
        a = x[2];
        b = y[2];
     }
     else {
        b = x[2];
        a = y[2];
     }
     
     return r;
}

// ------------------------------------------------------------------

bool inverso(INT_TYPE a, INT_TYPE b, INT_TYPE &inv) {
    INT_TYPE d, x = a, y = b;
    
    // a es invertible sii MCD(a, b) = 1
    d = mcd_ext(x, y);
    
    if(d == 1) {
        inv = x;
        if(inv < 0)
            inv = b + inv;

        return true;
    }
    
    return false;
}

// ------------------------------------------------------------------

INT_TYPE potencia_mod(INT_TYPE a, INT_TYPE m, INT_TYPE n) {
    INT_TYPE b = 1;
    
    if(!m) return b;
    
    // intentar optimizar usando menos operaciones %
    while(m > 0) {
        if(m & 0x1)
            b = (b*a) % n;
        a = (a*a) % n;
        m >>= 1;
    }
    
    //b = (b*a) % n;
    
    return b;
}

// ------------------------------------------------------------------

bool es_primo(INT_TYPE p) {
    INT_TYPE s, n = 2, a, aux; 
    
    // Casos base
    if(p == 3 or p == 2) return true;
    
    // Primero buscar "s" tq p-1 = 2^n * s
    while(aux == n or n > (p - 1)) {
        aux = mcd(n, p);
        n *= n;
    }
    
    std::cout << "#############  DEBUG  #############\n";
    std::cout << "## n = " << n << std::endl;
    
    // obtener s
    s = (p - 1) / n;
    // Comprobar si es mejor ver si s > p antes de hacer %
    s = s % p;
    
    // Debug
    
    std::cout << "## 2^" << n << " s: " << s << std::endl;

    // End debug
    
    // Generar de manera aleatoria a
    long unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator(seed);
    a = generator();        // Genera un nº entre 1 y 2147483646
    a %= (p-2);
    a = a < 1 ? 2 : a;
    
    a = potencia_mod(a, s, p);
    
    std::cout << "## marca 1\n";
    
    // Malo:
    // A partir de números > de 8 cifras los tiempos se disparan
    // Idea:
    // Desenrrollar bucle e intentar optimizar de otras maneras
    
    if (a == 1 or a == (p - 1))
        return true;
    else {
        for(INT_TYPE i = 1;i < (s - 1) and a != (p -1);++i) {
            a = (a * a) % p;
            if(a == 1) return false;
        }
        
        if(a != (p - 1)) return false;
    }
    
    return true;
}

// ------------------------------------------------------------------

bool primer_postulado_Golomb(std::bitset<BIT_SIZE> sec, bin_size longitud) {
    bin_size unos = 0,       // Indica el número de unos de la secuencia
            ceros = 0;      // Indica el número de ceros de la secuencia
            
    for(bin_size i = 0;i < longitud;++i)
        if(sec[i] == 1) ++unos;
        else ++ceros;

    if(abs(ceros - unos) > 1)
        return false;
    
    return true;
}

// ------------------------------------------------------------------

// Función para rotar una secuencia binaria n bits
// Revisar ---------------------

void rotar_sec(std::bitset<BIT_SIZE> &sec, bin_size, longitud, bin_size desp) {
    std::bitset<BIT_SIZE> sec_aux(sec);
    
    for(bin_size i = 0;i < longitud; ++i) {
        sec_aux[(i+desp)%longitud] = sec[i]; 
    }
    
    sec = sec_aux;
}

// ------------------------------------------------------------------

bool segundo_postulado_Golomb(std::bitset<BIT_SIZE> sec, bin_size longitud) {
    
    // Primero conseguir que no acabe por el mísmo símbolo que empieza
    // para facilitar las cosas
    if(sec[0] != sec[longitud-1]) {
        // Función lambda para ver de cuanto es la racha inicial para saber
        // de cuanto debe ser el desplazamiento para conseguir lo dicho antes
        // Se presupone que sec cumple el primer postulado de Golomb
        bin_size aux = [&sec, &longitud] () {
            bin_size aux = 1, i = 0;
            for( ;sec[0] == sec[i];++i, ++aux);
            
            return aux;
        };
        
        rotar_sec(sec, longitud, aux);
    }
    
    // Declaración de una función anónima para calcuar la longitud de una racha
    // desde la posición i
    auto calcula_rachas = [&sec] (int &i) {
            bin_size aux = i + 1;
            for( ;sec[i] == sec[aux];++i, ++aux);
            
            return aux;
    };
    
    bin_size i = 0;
    std::map<int, int> rachas;
    
    // Inicializar el map con las longitudes de rachas posibles
    for(bin_size j = 0;j < (longitud>>1);++j)
        rachas.insert( std::pair<int, int>(i, 0) );
    
    while(i < longitud) {
        rachas[calcula_rachas(i)]++;
    }
    
    // Comprobar que las rachas cumplen el 2º postulado de Golomb
    for(bin_size j = 0;j < (longitud>>1);++j)
        if(rachas[j] != (rachas[j+1]<<1) or rachas[j] != rachas[j+1])
            return false;
    
    return true;
}

// ------------------------------------------------------------------

bin_size distancia_Hamming(std::bitset<BIT_SIZE> sec1, 
                           std::bitset<BIT_SIZE> sec2, bin_size longitud) {
    /*
       En vez de ir comprobando posición por posición si sec1[i] == sec2[i]
       utilizo una secuencia auxiliar construida a partir de la XOR de las
       otras dos, de manera que solo tengo que ir sumando los unos de aux
       para saber la distancia de Hamming.
    */
    bin_size dist = 0;
    std::bitset<BIT_SIZE> aux(sec1^sec2);
    
    for(bin_size i = 0;i < longitud;++i) {
        dist += aux[i];
    }
    
    return dist;
}

// ------------------------------------------------------------------

bool tercer_postulado_Golomb(std::bitset<BIT_SIZE> sec, bin_size longitud) {
    std::
    bin_size dist = distancia_Hamming(sec, );
    
    for(bin_size i = 0;i < longitud;++i) {
        
    }
}

// ------------------------------------------------------------------

bool cumple_postulados_Golomb(std::bitset<BIT_SIZE> sec, bin_size longitud) {
    bool cumple = false;
    
    cumple = primer_postulado_Golomb(sec, longitud);
    if(!cumple) return cumple;
    
    cumple = segundo_postulado_Golomb(sec, longitud);
    if(!cumple) return cumple;
    
    cumple = tercer_postulado_Golomb(sec, longitud);
    
    return cumple;
}


// Final fichero: libcrip.cpp
