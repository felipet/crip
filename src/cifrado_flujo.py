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

def cifrado_flujo(fichero):
    """Cifrado en flujo usando el generador de Geffe"""
    
    # La complejidad lineal del conjunto sería mcm(4,5,3)
    # Asumo que las líneas del fichero son de 32 caracteres
    r1 = bv.BitVector( bitstring = "1010")
    r2 = bv.BitVector( bitstring = "10101")
    r3 = bv.BitVector( bitstring = "110")
    s1 = bv.BitVector( bitstring = "1001")
    s2 = bv.BitVector( bitstring = "10110")
    s3 = bv.BitVector( bitstring = "101")
    
    cadena1 = lfsr(r1, s1, 32)
    cadena2 = lfsr(r2, s2, 32)
    cadena3 = lfsr(r3, s3, 32)
    
    temp1 = cadena1 & cadena2
    temp2 = ~cadena2 & cadena3
    key = temp1 ^ temp2       # Esta es la clave
    
    f = open(fichero, 'r')
    
    for line in f:
        cad = bv.BitVector( bitstring = line.rstrip() )
        cad = cad ^ key
        print(cad)
        

    

def main():
#    coefs = str((input("Introducir los coefs del c(D): ")))
#    coeficientes = bv.BitVector( bitstring = coefs )
#    seed = str((input("Introducir cadena de semilla: ")))
#    semilla = bv.BitVector( bitstring = seed )
#    k = int((input("Introducir longitud de salida: ")))
#    
#    resultados = lfsr(coeficientes, semilla, k)
#    print(resultados)
    filename = str((input("Introducir el nombre del fichero: ")))
    cifrado_flujo(filename)

if __name__ == '__main__':
    main()
        
