#!/usr/bin/python

import sys
import BitVector as bv

def berlekamp_massei(sucesion):
    """Calcula la longitud mínima de un LFSR que genere la sucesión y su c(D)"""
    
    # Inicializar las variables
    n = sucesion.length()
    g = bv.BitVector( intVal = 1, size = n ) # Construyo g con el MSB a 1
    f = bv.BitVector( intVal = 3, size = n ) # Construyo f para el polinomio x+1
    f = f.reverse()
    g = g.reverse()
    
    
    # Tomamos k el primer entero tq suscesion(k) = 1
    for i in range(n):
        if sucesion[i] == 1:
            k = i;
            break;
    
#    print("f = \t", f)
#    print("g = \t", g)
#    print("k = \t", k)
    
    r = l = k + 1;
    a = k;
    b = 0;
    
    
    while r < n:
        d = 0
        for i in range(l+1):   # i <= l
            d ^= f[i] * sucesion[i+r-l]
        
        if d == 0:
            b += 1
        if d == 1:
            if 2*l > r:
                #for i in range(l+1):    # La interación l entra 
                    #f[i] = f[i] ^ g[i+b-a]
                f = f ^ (g << (b-a) )
                b += 1
            
            else:
                aux = f.deep_copy()
                
               # for i in range(r+l):     # Ver si hay que añadir -1
                     #f[i] = aux[i+(a-b)] ^ g[i]
                f = g ^ (aux >> (b-a) )
                     
                l = r - l + 1
                g = aux.deep_copy() << (b - a)
                a = b
                b = r - l + 1
        r += 1
         
#    print("Resultado L = ", l)
#    print("Cadena      = ", f)
    return f,l
    
def formatea_polinomio_conexion(sucesion, size):
    """Da formato a un BitVector como c(D) """

    salida = "Polinomio de conexión c(D) = "
    for i in range(size, 0, -1):
        if sucesion[i] == 1:
            salida += "D^" + str(i) + " + "
    salida += "1"
    print(salida)
    

def main():
    cadena = str((input("Introducir una secuencia : ")))
    sucesion = bv.BitVector( bitstring = cadena )
    #sucesion = bv.BitVector( bitstring = '11011001010000111011' )
    
    resultados = berlekamp_massei(sucesion)
    print(resultados[0])
    formatea_polinomio_conexion(resultados[0], resultados[1])
    print("L = ", resultados[1])

if __name__ == '__main__':
    main()
