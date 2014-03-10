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

using namespace boost::multiprecision;

typedef int128_t INT_TYPE; //< Plantilla para los enteros


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

#endif

// Final de fichero: libcrip.hpp
