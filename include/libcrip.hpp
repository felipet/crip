/**
 * \file libcrip.hpp
 * \brief Fichero de cabecera para la biblioteca de CRIP
 * \author Felipe Torres González
 * \version 0.1
 * \date 2014
 * \copyright Apache v2.0
 *
 * Implementaciones para los ejercicios de la práctica 1 de Criptografía
 * y computación.
 *
 */
 
#ifndef __LIBCRIP_HPP__
#define __LIBCRIP_HPP__

#include <boost/multiprecision/cpp_int.hpp>
#include <bitset>

#define BIT_SIZE 32

using namespace boost::multiprecision;

typedef int128_t INT_TYPE; //< Plantilla para los enteros
typedef uint8_t bin_size;  //< Tipo de datos para los tamaños de las cadenas binarias
typedef long long unsigned big_unsigned;


/**
 * \brief Máximo común divisor de dos enteros
 * Calcula el MCD de dos enteros utilizando el algoritmo de
 * Euclides.
 * \param[in] a Entrada del primer entero. 
 * \param[in] b Entrada del segundo entero.
 * \return El MCD de a y b.
 *
 */
INT_TYPE mcd(INT_TYPE a, INT_TYPE b);

/**
 * \brief Máximo común divisor de dos enteros
 * Calcula el MCD de dos enteros utilizando el algoritmo extendido de
 * Euclides. Además resuelve d = ax + by
 * \param[in,out] a Entrada del primer entero, a. Salida de x.
 * \param[in,out] b Entrada del segundo entero, b. Salida de y.
 * \return El MCD de a y b.
 *
 */
INT_TYPE mcd_ext(INT_TYPE &a, INT_TYPE &b);

/**
 * \brief Calcular a^(-1) mod b
 * \param[in] a Entero al que se le calcula el inverso
 * \param[in] b Módulo
 * \param[out] inv inverso de a mod b
 * \pre a y b deben ser primos relativos
 * \retval true si a tiene inverso
 * \retval false si no tiene inverso
 */
bool inverso(INT_TYPE a, INT_TYPE b, INT_TYPE &inv);

/**
 * \brief Calcular a^m mod n
 * \param[in] a Base
 * \param[out] m Exponente
 * \param[in] n Módulo
 * \return a^m mod n
 * \pre m >= 1
 * \pre n >= 2
 */
INT_TYPE potencia_mod(INT_TYPE a, INT_TYPE m, INT_TYPE n);

/**
 * \brief Test de primalidad Miller-Rabin
 * Si se quiere aumentar el nivel de certeza para evitar falsos positivos
 * repetir el algoritmo varias veces. Para números pequeños con 5 iteraciones
 * se obtiene un buen nivel de certeza, para grandes 10 está bien.
 * \param[in] p valor por determinar si es primo
 * \retval false el valor es 100% no primo
 * \retval true el valor es primo con un % de certeza
 * \pre p >= 1
 */
bool es_primo(INT_TYPE p);

/**
 * \brief Logaritmo discreto
 * Calcula el valor de b en la expresión a^b = c mod p
 * \param[in] a base
 * \param[in] c valor resultado
 * \param[in] p primo para el módulo
 * \return el valor del logaritmo que resuelve la expresión anterior
 * \pre p es primo
 */
INT_TYPE log_discreto(INT_TYPE a, INT_TYPE c, INT_TYPE p);

/**
 * \brief Método de Fermat para factorizar enteros
 * \param[in] n el número entero a factorizar
 * \param[out] x devolverá el valor calculado para uno de los factores
 * \param[out] y devolverá el valor calculado para el otro de los factores
 * \retval true si el número no es primo
 * \retval false si el número es primo y no se puede factorizar
 * \post x e y solo tendrán valores válido si la función devuelve true
 */
bool metodo_fermat(INT_TYPE n, INT_TYPE &x, INT_TYPE &y);

/**
 * \brief Método de factorización de enteros rho-Pollard
 * \param[in] n el número entero a factorizar
 * \param[in] i máximas iteraciones del bucle (condición de parada)
 * \return un factor de n
 * \retval 1 si n es probablemente primo
 * \retval 0 si se salió del bucle antes de encontrar un factor
 * \pre
 */
INT_TYPE metodo_rho_pollard(INT_TYPE n, unsigned iter = 10000000);

/**
 * \brief Función que determina si una secuencia binaria cumple el primer postulado de Golomb.
 * \param[in] sec una secuencia de bits del tipo bitset de la STL
 * \param[in] longitud número de posiciones válidas en sec
 * \retval false si no cumple el primer postulado
 * \retval true si cumple el primer postulado
 * \pre longitud debe ser <= que BIT_SIZE
 */

bool primer_postulado_Golomb(std::bitset<BIT_SIZE> sec, bin_size longitud);

/**
 * \brief Función que determina si una secuencia binaria cumple el segundo postulado de Golomb
 * \param[in] sec una secuencia de bits del tipo bitset de la STL
 * \param[in] longitud número de posiciones válidas en sec
 * \retval false si no cumple el primer postulado
 * \retval true si cumple el primer postulado
 * \pre longitud debe ser <= que BIT_SIZE
 */
 
bool segundo_postulado_Golomb(std::bitset<BIT_SIZE> sec);

/**
 * \brief Función que determina si una secuencia binaria cumple el tercer postulado de Golomb
 * \param[in] sec una secuencia de bits del tipo bitset de la STL
 * \param[in] longitud número de posiciones válidas en sec
 * \retval false si no cumple el primer postulado
 * \retval true si cumple el primer postulado
 * \pre longitud debe ser <= que BIT_SIZE
 */
 
bool tercer_postulado_Golomb(std::bitset<BIT_SIZE> sec);

/**
 * \brief Función que determina si una secuencia de bits cumple los postulados de Golomb.
 * \param[in] 
 * \retval false
 * \retval true
 * \pre
 */
bool cumple_postulados_Golomb(std::bitset<BIT_SIZE> sec);


#endif

// Final de fichero: libcrip.hpp
