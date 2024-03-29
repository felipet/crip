#include "libcrip.hpp"
#include <random>
#include <chrono>
#include <map>
#include <tuple>
#include <fstream>
#include <string>
#include <iostream>

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

void rotar_sec(std::bitset<BIT_SIZE> &sec, bin_size longitud, bin_size desp) {
    std::bitset<BIT_SIZE> sec_aux(sec);
    
    for(bin_size i = 0;i < longitud; ++i)
        sec_aux[i] = sec[(i+desp)%longitud];
    
    sec = sec_aux;
}

// ------------------------------------------------------------------

bool segundo_postulado_Golomb(std::bitset<BIT_SIZE> sec, bin_size longitud) {
    
    // Primero conseguir que no acabe por el mísmo símbolo que empieza
    // para facilitar las cosas
    if(sec[0] == sec[longitud-1]) {
        // Función lambda para ver de cuanto es la racha inicial para saber
        // de cuanto debe ser el desplazamiento para conseguir lo dicho antes
        // Se presupone que sec cumple el primer postulado de Golomb
        auto racha = [&sec, &longitud] () {
            bin_size aux = 1, i = 1;
            for( ;sec[0] == sec[i];++i, ++aux);
            
            return (bin_size) aux;
        };
        
        rotar_sec(sec, longitud, racha());
    }

    std::map<int, int> mapa;
    short unsigned actual = sec[0];
    short unsigned cont = 1;
    
    // Calcular rachas
    for(bin_size i = 1;i < longitud; i++) {
        if(sec[i] == actual and i != longitud-1) cont++;
        else {
            actual ^= 0x1;
            if(i == longitud - 1)
                cont++;
            mapa[cont]++;
            cont = 1;
        }
    }
    
    
    // Comprobar rachas
    for(bin_size i = 1;i < mapa.size();i++)
        if(mapa[i] != (mapa[i+1]<<1) and mapa[i+1] != 1) 
            return false;
        
    
    return true;
}

// ------------------------------------------------------------------

unsigned distancia_Hamming(std::bitset<BIT_SIZE> sec1, 
                           std::bitset<BIT_SIZE> sec2) {
    /*
       En vez de ir comprobando posición por posición si sec1[i] == sec2[i]
       utilizo una secuencia auxiliar construida a partir de la XOR de las
       otras dos, de manera que solo tengo que devolver la cuenta de 1 que
       hay en la nueva secuencia.
       
       No se tiene en cuenta la longitud pues el espacio de sobra son 0s
    */
    std::bitset<BIT_SIZE> aux(sec1^sec2);

    return aux.count();
}

// ------------------------------------------------------------------

bool tercer_postulado_Golomb(std::bitset<BIT_SIZE> sec, bin_size longitud) {
    std::bitset<BIT_SIZE> sec_aux(sec);
    rotar_sec(sec_aux, longitud, 1);
    
    unsigned ini_dist = distancia_Hamming(sec, sec_aux);

    for(bin_size i = 1;i < longitud-1; i++) {
        rotar_sec(sec_aux, longitud, 1);
        
        if(ini_dist != distancia_Hamming(sec, sec_aux))
            return false;
    }

	return true;
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

// ------------------------------------------------------------------

void despl_izq(std::bitset<BIT_SIZE> &sec, bin_size longitud) {
    for(bin_size i = longitud-1;i > 0;i--) 
        sec[i] = sec[i-1];
        
    sec[0] = 0;
}

// ------------------------------------------------------------------

bool 
suma_z2(std::bitset<BIT_SIZE> op1,
        std::bitset<BIT_SIZE> op2, bin_size longitud) {
                              
    std::bitset<BIT_SIZE> suma;
    
    for(bin_size i = 0;i < longitud;i++)
        suma[i] = op1[i] and op2[i];
        
    bool a = suma.count() %2;
    
    //std::cout << "la suma es " << a << std::endl;
    
    return a;
}       

// ------------------------------------------------------------------

void reverse_bitset(std::bitset<BIT_SIZE> &sec, bin_size longitud) {
    std::bitset<BIT_SIZE> aux;
    
    for(bin_size i = 0;i < longitud;i++) {
        aux[i] = sec[longitud-i];
    }
    
    sec = aux;
}

// ------------------------------------------------------------------

namespace std{
bitset<BIT_SIZE> op_and(bitset<BIT_SIZE> sec1, bitset<BIT_SIZE> sec2, bin_size longitud) {
    
    bitset<BIT_SIZE> aux;
    
    for(bin_size i = 0;i < longitud;i++)
        aux[i] = sec1[i] * sec2[i];
    
    return aux;
}
}

// ------------------------------------------------------------------

int LFSR(  std::bitset<BIT_SIZE> coefs, unsigned size_coefs, 
           std::bitset<BIT_SIZE> seed, unsigned size_seed,
           std::bitset<BIT_SIZE> &out, unsigned size_out) {
            
    if(size_out >= BIT_SIZE) return -1;
    
    //out = seed;
    //out <<= 1;
    //std::cout << "out \t" << out << std::endl;
    //std::cout << "seed \t" << seed << std::endl;
    
    for(unsigned i = 0;i < size_out;i++) {
        out[0] = suma_z2(coefs, seed, size_coefs);
        //std::cout << "out \t" << out << std::endl;
        despl_izq(seed, size_seed);
        seed[0] = out[0];
        if(i < size_out - 1)
            out <<= 1;
        //std::cout << "seed \t" << seed << std::endl;
    }
    
    return 0;
}

// ------------------------------------------------------------------

void rango_bitset( std::bitset<BIT_SIZE> org, unsigned pos_ini, unsigned pos_fin,
                   std::bitset<BIT_SIZE> &out) {
    std::bitset<BIT_SIZE> aux;
    
    for(unsigned i = 0;i < pos_fin-pos_ini;i++)
        aux[i] = org[i+pos_ini];
        
    out = aux;
}

// ------------------------------------------------------------------

int NLFSR(  std::bitset<BIT_SIZE> lista_monomios,
            unsigned size_lista,
            unsigned num_variables,
            std::bitset<BIT_SIZE> semilla,
            unsigned k,
            std::bitset<BIT_SIZE> &sec_out)                      {
    
    std::bitset<BIT_SIZE> aux;
    unsigned cont = 0;
    
    for(unsigned i = 0;i < k;i++) {
        std::bitset<BIT_SIZE> temp;
            for(unsigned j = 0;j < size_lista;j++) {
                // TODO: la lista_monomios se lee al revés
                rango_bitset(lista_monomios, cont, cont+4, temp);
                
                std::cout << "temp generado : \t\t" << temp << std::endl;
                
                temp ^= semilla;
            
                std::cout << "temp + semilla : \t\t" << temp << std::endl;
            
                cont += 4;  
            }
        
        cont = 0;
        aux = temp.count() % 2;
        aux <<= 1;
    }
    
    sec_out = aux;
     
    return 0;      
}

// ------------------------------------------------------------------


int cifrado_flujo(std::string in_file, std::string out_file) {
    if(out_file.empty())    return -2;
    
    char line[BIT_SIZE];
    std::ifstream input(in_file);
    // TODO: NO ABRE ESTE PUTO ARCHIVO
    //if(!input.is_open()) return -1;
    std::fstream output(out_file, std::fstream::out | std::fstream::trunc);
    std::bitset<BIT_SIZE> lfsr1, lfsr2, lfsr3;
    unsigned size1, size2, size3, size_m;
    
    LFSR( std::bitset<BIT_SIZE>("1111"), 4, std::bitset<BIT_SIZE>("1101"),
          4,lfsr1, 5); // D4+D3+D2+D+1 con semilla 1101, período 5
    LFSR( std::bitset<BIT_SIZE>("1010"), 4, std::bitset<BIT_SIZE>("1001"),
          4,lfsr2, 6); // D4+D2+1 con semilla 1001, período 6
    LFSR( std::bitset<BIT_SIZE>("1010"), 4, std::bitset<BIT_SIZE>("1101"),
          4,lfsr3, 3); // D4+D2+1 con semilla 1101, período 3
          
    std::cout << "lfsr1 " << lfsr1 << std::endl;
    std::cout << "lfsr2 " << lfsr2 << std::endl;
    std::cout << "lfsr3 " << lfsr3 << std::endl;
    
    while( input.getline(line, BIT_SIZE, '\n') ) {
        std::bitset<BIT_SIZE> msg(line);
        int error = generador_Geffe(lfsr1, lfsr2, lfsr3, std::max(std::max      
                    (size1, size2),size3), msg, size_m);
        if(error > 0) return -3;
        output << msg.to_string() << "\n";
    }
    
    output.close();
    input.close();
    
    return 0;
}

// ------------------------------------------------------------------

 int generador_Geffe( std::bitset<BIT_SIZE> lfsr1, 
                      std::bitset<BIT_SIZE> lfsr2, 
                      std::bitset<BIT_SIZE> lfsr3, unsigned size_max,
                      std::bitset<BIT_SIZE> &msg, unsigned size_m ) {
    
    //TODO: Comprobar que size_max están dentro de rangos...
          
    std::bitset<BIT_SIZE> temp1 = op_and(lfsr1,lfsr2, size_max);
    std::bitset<BIT_SIZE> temp2 = op_and(~lfsr2, lfsr3, size_max);
    std::bitset<BIT_SIZE> temp3 = temp1^temp2;
    std::cout << "temp 3 " << temp3 << std::endl;
    
    return 0;
}

// ------------------------------------------------------------------

 int berlekamp_massey(std::bitset<BIT_SIZE> &sec, unsigned longitud, 
                       unsigned &complejidad) {
                       
    unsigned k = 0;
    std::bitset<BIT_SIZE> f, g, aux;
    unsigned l,a,b,r,d = 0;
     
    if(longitud < 2) return -1;
     
    while(k < longitud and sec[k] != 1) ++k;
    std::cout << "\tK = " << k << std::endl;
    
    g[longitud-1] = 1;
    l = r = (k + 1) % 2;
    a = k;
    b = 0;
    f[longitud-1] = r;
    f[longitud-2] = l;
    std::cout << "\tDatos iniciales : " << std::endl;
    std::cout << "\tg : " << g << std::endl;
    std::cout << "\tf : " << f << std::endl;
    std::cout << "\tr,l,a,b : " << r << "," << l << "," << a << "," << b << std::endl;
    
    
    while(r < longitud) {
        std::cout << "\tIteración : " << r << std::endl;
        d = 0;
        
        std::bitset<BIT_SIZE> q(f), t("11001000111101");
        reverse_bitset(q, longitud);
        q >>= 1;
        //reverse_bitset(t, longitud);
        //t = "11001000111101";
        std::cout << "\tq : " << q << std::endl;
        std::cout << "\tt : " << t << std::endl;
        for(unsigned i = 0;i <= l;i++) {
            //d += f[longitud-i] * sec[longitud-i+r-l]; 
            d += q[i] * t[i+r-l];
            std::cout << "\td = " << d << std::endl;
        }
        d = d%2;
        
        if(!d) {
            b++;
            std::cout << "\t\tD==0" << std::endl;
        }
            
        if(d == 1) {
            if((l<<1) > r) {
                /*
                for(unsigned i = 0;i <= l;i++)
                    f[longitud-i] = f[longitud-i] ^ g[longitud-i+b-a];
                */
                f = f ^ (g << (b-a));
                b++;
                std::cout << "\t\tD==1 and 1" << std::endl;
            }
            else {
                aux = f;
                std::cout << "\taux : \t" << aux << std::endl;
                /*
                for(unsigned i = 0;i <= r+l-1;i++)
                    f[longitud-i] = aux[longitud-i+(a-b)] ^ g[longitud-i];
                */
                f = aux ^ (g << (b-a) );
                l = r+1-l;
                g = aux << 1;
                a = b;
                b = r+1-l;
                
                std::cout << "\t\tD==1 and 2" << std::endl;
            }
        }
        r++;
        
        std::cout << "\tDatos : " << std::endl;
    std::cout << "\tg : \t" << g << std::endl;
    std::cout << "\tf : \t" << f << std::endl;
    std::cout << "\tr,l,a,b : " << r << "," << l << "," << a << "," << b << std::endl;
    }
    
    sec = f;
    complejidad = l;
     
    return 0;
}

// Final fichero: libcrip.cpp
