#include "libcrip.hpp"
#include <chrono>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    INT_TYPE a(argv[1]),
             b(argv[2]),
             x = a,y = b,d, inv;
    
    auto t1 = std::chrono::high_resolution_clock::now();         
    d = mcd_ext(x,y);
    auto t2 = std::chrono::high_resolution_clock::now();
    
    std::cout << "Resultados: \nMCD de " << a << " y " << b << " :" << std::endl;
    std::cout << d << " = " << a << " * " << x << " + " << b << " * ";
    std::cout << y << std::endl;
    if(a*x+b*y == d)
        std::cout << "Resultado Correcto\n";
    std::cout << "Tiempo : " << std::chrono::duration_cast<std::chrono::microseconds >
                 (t2 - t1).count() << "us" << std::endl;
        
    std::cout << "El inverso de " << a << " mod " << b << " es :" << std::endl;
    if(inverso(a,b, inv)) std::cout << inv << std::endl;
    else std::cout << "No tiene inverso" << std::endl;
    
    return 0;
}
