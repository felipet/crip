#include "libcrip.hpp"
#include <random>
#include <chrono>
#include <iostream>
#include <vector>


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

INT_TYPE log_discreto(INT_TYPE a, INT_TYPE c, INT_TYPE p) {
    INT_TYPE t, r, aux;
    big_unsigned s; // Se asegura que s será de almenos 64 bits
    
    // Este casting es una lotería (probado con números pequeños)
    s = (big_unsigned) sqrt(p) + 1; // Sumar uno por el redondeo al hacer la raíz entera

//  BUG: el tamaño de la reserva no puede ser pasado desde un tipo no integral
//       por lo que s no puede ser de los tipos usados en Boost.
    INT_TYPE *vt, *vr;
    vt = new INT_TYPE[s];
    vr = new INT_TYPE[s];
    
    std::cout << "## Debug : \n";
    std::cout << "\t a : " << a << std::endl;
    std::cout << "\t p : " << p << std::endl;
    std::cout << "\t c : " << c << std::endl;
    std::cout << "\t s : " << s << std::endl;
    
    // Poner aquí por qué es hasta s
    vr[0] = c;
    //vt[0] = aux = (a^s) % p;
    vt[0] = aux = potencia_mod(a, s, p);
        
    // ### Paralelizar o no
    // Vector paso enano
    for(big_unsigned i = 1;i < s;++i) {
        vr[i] = (vr[i-1] * a) % p;
    }
    
    // Vector paso gigante
    INT_TYPE x = potencia_mod(a, s, p);
    for(long long unsigned i = 1;i < s;++i) {
        vt[i] = (vt[i-1] * aux) % p;
    }
    
    // Función anómima que indica si un elemento del vector vr se encuentra
    // en vt y en caso afirmativo devuelve la posición donde lo encontró.
    auto encontrado = [&] (INT_TYPE elem) {
        for(big_unsigned i = 0;i < s;++i)
            if(vt[i] == elem) 
                return i+1; // más uno porque los índices en vt empiezan en 1
            
        return (big_unsigned) 0;
    };
    
    t = 0;
    big_unsigned temp;
    for(big_unsigned i = 0;i < s;++i) {
        temp = encontrado(vr[i]);
        if(temp) {
            std::cout << "## Debug : \n";
            std::cout << "\t temp : " << temp << std::endl;
            std::cout << "\t s : " << s << std::endl;
            std::cout << "\t i : " << i << std::endl;
            
            t = s * temp - i;
            std::cout << "\t t : " << t << std::endl;
            break;
        }
    }
    
    std::cout << "## Debug : \n";
    for(big_unsigned i = 0;i < s;++i) {  
        std::cout << "\tvt : " << vt[i] << " ,vr : " << vr[i] << std::endl;
    }
    
    delete[] vr;
    delete[] vt;
    
    return t;
}


// Final fichero: libcrip.cpp
