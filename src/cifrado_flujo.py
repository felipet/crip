#!/usr/bin/python

import sys
import BitVector as bv

def lfsr(p_conexion, semilla, size_out):
    """Implementación de un LFSR
       
       Los coefs del p_conexión se introducen de mayor a menor y no el 1 final
    """
    
    # La semilla no debe tenerl el 1 final del c(D)
    out = bv.BitVector( size = size_out )
    r = 0
    
    while(r < size_out):
        out[0] = (p_conexion & semilla).count_bits() % 2
        semilla <<= 1
        semilla[-1] = out[0]
        out <<= 1
        r += 1
    
    return out



def main():
    coefs = str((input("Introducir los coefs del c(D): ")))
    coeficientes = bv.BitVector( bitstring = coefs )
    seed = str((input("Introducir cadena de semilla: ")))
    semilla = bv.BitVector( bitstring = seed )
    k = int((input("Introducir longitud de salida: ")))
    
    resultados = lfsr(coeficientes, semilla, k)
    print(resultados)

if __name__ == '__main__':
    main()
        
