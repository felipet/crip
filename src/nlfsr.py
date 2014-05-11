#!/usr/bin/python

import sys
import BitVector as bv

def nlfsr(coeficientes, n_variables, semilla, k):
    """Implementa un NLFSR"""
    lista_coefs = []
    out = bv.BitVector( size = k )
    r = 0
    
    #Almaceno las sumas de monomios independientemente
    for i in range(0, n_variables*n_variables, n_variables):
        lista_coefs.append( coeficientes[i:i+4] )
    
    while(r < k): 
        aux = semilla.deep_copy()
        l2 = []
        suma = 0
        for i in range(n_variables):
            aux = aux & lista_coefs[i]
            l2.append(aux)
            aux = semilla.deep_copy()
            
        aux.reset(0)
        for i in l2:
            aux = aux | i
        
        semilla <<= 1
        out[r] = semilla[n_variables-1] = aux.count_bits() % 2
        r += 1
        
    return out
    

def main():
    coefs = str((input("Introducir cadena de coeficientes: ")))
    coeficientes = bv.BitVector( bitstring = coefs )
    var = int((input("Introducir número de variables: ")))
    seed = str((input("Introducir cadena de semilla: ")))
    semilla = bv.BitVector( bitstring = seed )
    k = int((input("Introducir longitud de salida: ")))
    
    resultados = nlfsr(coeficientes, var, semilla, k)
    print(resultados)

if __name__ == '__main__':
    main()
