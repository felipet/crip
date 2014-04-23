#include "libcrip.hpp"
#include <random>
#include <chrono>
#include <map>
#include <stdio.h>

//#define DEBUG

INT_TYPE MCD(INT_TYPE a, INT_TYPE b) {
    INT_TYPE r;
    
    if(a < b) {
        a ^= b;
        b ^= a;
        a ^= b;
    }
    
    while(b > 0) {
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
        aux = MCD(n, p);
        n *= n;
    }
    
    // obtener s
    s = (p - 1) / n;
    // Comprobar si es mejor ver si s > p antes de hacer %
    s = s % p;
    
    // Generar de manera aleatoria a
    long unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator(seed);
    a = generator();        // Genera un nº entre 1 y 2147483646
    a %= (p-2);
    a = a < 1 ? 2 : a;
    
    a = potencia_mod(a, s, p);
    
    // Malo:
    // A partir de números > de 8 cifras los tiempos se disparan
    
    if (a == 1 or a == (p - 1))
        return true;
    else {
        for(INT_TYPE i = 1;i < (s - 1);++i) {
            a = (a * a) % p;
            if(a == 1) return false;
            if(a == (p-1)) return true;
        }
    }
    
    return false;
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
    
    // Poner aquí por qué es hasta s
    vr[0] = c;
    //vt[0] = aux = (a^s) % p;
    vt[0] = aux = potencia_mod(a, s, p);
        
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
            t = s * temp - i;
            break;
        }
    }
    
    delete[] vr;
    delete[] vt;
    
    return t;
}

// ------------------------------------------------------------------

bool metodo_fermat(INT_TYPE n, INT_TYPE &x, INT_TYPE &y) {
    INT_TYPE j = sqrt(n) + 1,           // sqrt coge el entero inferior
             aux;
    
    // f(x) = x^2 -n
    auto f = [&n] (INT_TYPE x) { return x * x - n;};
    
    // Función lambda para determinar si k es cuadrado perfecto
    // Revisar esta función
    auto cuadrado_perfecto = [] (INT_TYPE k) { 
        INT_TYPE square_root = sqrt(k);
        return (square_root * square_root == k);
    };
    
    aux = f(j);
    while(!cuadrado_perfecto(aux) and j < n) {
        ++j;
        aux = f(j);
    }
    
    if(j < n-1) {
        x = j;
        y = sqrt( f(j) );
        return true;
    }
    
    return false;
}

// ------------------------------------------------------------------

INT_TYPE metodo_rho_pollard(INT_TYPE n, unsigned iter) {
    INT_TYPE a = 5975, x, y, d, r, p;
    unsigned i = 0;
    
    // Función pseudoaleatoria
    auto f = [&n](INT_TYPE x) { return (x * x + 1) % n; };
    
    x = f(a);
    y = f(x);
    
    while(i < iter) {
        r = y - x;
        
        // El operador % no trabaja bien con números < 0
        if(r < 0) r = n + r;
        else r %= n;
        
        p = n;
        d = mcd_ext(r, p);
        
        if(d == n) {
            return 1;
        }
        if(d == 1) {
            //++i;
            x = f(x);
            y = f( f(y) );
        }
        else if(n % d == 0) {
            return d;
        }
        
        ++i;
    }
    
    return 0;
}

// ------------------------------------------------------------------

bool primer_postulado_Golomb(std::bitset<BIT_SIZE> sec, bin_size longitud) {
    bin_size unos = 0,       // Indica el número de unos de la secuencia
            ceros = 0;      // Indica el número de ceros de la secuencia
            
    for(bin_size i = 0;i < longitud; ++i)
        if(sec[i] == 1) ++unos;
        else ++ceros;

    if(abs(ceros - unos) > 1)
        return false;
    
    return true;
}

// ------------------------------------------------------------------

// Función para rotar una secuencia binaria n bits
// Revisar ---------------------

void rotar_sec(std::bitset<BIT_SIZE> &sec, bin_size longitud, bin_size desp) {
    std::bitset<BIT_SIZE> sec_aux(sec);
    
    for(bin_size i = 0;i < longitud; ++i) {
        sec_aux[(i+desp)%longitud] = sec[i]; 
    }
    
    sec = sec_aux;
}

// ------------------------------------------------------------------

bool segundo_postulado_Golomb(std::bitset<BIT_SIZE> sec, bin_size longitud) {

    std::cout << "## Debug ##" << std::endl;
    
    // Primero conseguir que no acabe por el mísmo símbolo que empieza
    // para facilitar las cosas
    std::cout << "Cadena ini:" << sec << std::endl;
    printf("sec[0] = %d, sec[%d] = %d\n",(int)sec[0], longitud-1, (int)sec[longitud-1]);
    printf("Longitud : %d\n", (int) longitud);

    if(sec[0] == sec[longitud-1]) {
    
    // Debug
    std::cout << "\tEmpieza igual que acaba : " << sec << std::endl;
    std::cout << "\tLongitud : " << (unsigned) longitud << std::endl;
    
        // Función lambda para ver de cuanto es la racha inicial para saber
        // de cuanto debe ser el desplazamiento para conseguir lo dicho antes
        // Se presupone que sec cumple el primer postulado de Golomb
        auto aux = [&sec, &longitud] () {
            bin_size aux = 1, i = 1;
            for( ;sec[0] == sec[i];++i, ++aux);
            
            return (bin_size) aux;
        };
        
        //debug
        bin_size temp = aux();
        printf("Desplazamiento inicial : %d\n", (int) temp);
        
//        rotar_sec(sec, longitud, aux());
        rotar_sec(sec, longitud, temp);
        
        std::cout << "\tLa nueva cadena es " << sec << std::endl;
    }
    
    // Declaración de una función anónima para calcuar la longitud de una racha
    // desde la posición i
//    auto calcula_rachas = [&sec] (bin_size &i) {
//            bin_size aux = i + 1;
//            if(sec[i] != sec[i+1] and sec[i] == sec[i+2]) return (bin_size) 1;
//
//            for( ;sec[i] == sec[aux];++i, ++aux);
//
//            return aux;
//    };

    auto calcula_rachas = [&sec, &longitud] (bin_size &k) {
    	bin_size i = 1, racha = 0;

    	for(bin_size j = 0;j < longitud and racha < k; ++j) {
			if(sec[j] == sec[i])
				racha++;
			else return racha;
    	}

    	return racha;
    };

    bin_size i = 0;
    std::map<int, int> rachas;
    
    // Inicializar el map con las longitudes de rachas posibles
    for(bin_size j = 1;j < (longitud>>1); ++j) {
        rachas.insert( std::pair<int, int>(j, 0) );
        std::cout << (int) j << " " << rachas.at(j) << std::endl;
    }
    
    printf("He pasado de aquí\n");
    std::cout << rachas.size() << std::endl;
    
    i = 1;
    bool pass = false;
    while(i <= longitud) {
        rachas[calcula_rachas(i)]++;

        if(pass) {
        	++i;
        	pass ^= 1;
        }
        else pass ^= 1;
    }
    
    printf("Y de aquí\n");
    std::cout << "rachas de 4:" << rachas[4] << std::endl;
    std::cout << "rachas de 3:" << rachas[3] << std::endl;
    std::cout << "rachas de 2:" << rachas[2] << std::endl;
    std::cout << "rachas de 1:" << rachas[1] << std::endl;

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
//    std::
//    bin_size dist = distancia_Hamming(sec, );
//    
//    for(bin_size i = 0;i < longitud;++i) {
//        
//    }

	return false;
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
