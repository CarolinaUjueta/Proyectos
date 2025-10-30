/*****************************************************************************************************
 *                                   PONTIFICIA UNIVERSIDAD JAVERIANA                                *
 *                     Departamento de Ingeniería de Sistemas – Sistemas Operativos                  *
 *                                                                                                   *
 * ------------------------------------------------------------------------------------------------- *
 * Autor       : Thomas Leal, Carolina Ujueta                                                        *
 * Fecha       : 27/10/2025                                                                          *
 * Objetivo    : 4 procesos con fork(); cada hijo lee su archivo y calcula la suma total de enteros. *
 *               Padre recibe por pipe el resultado.                                                 *
 *                                                                                                   *
 *****************************************************************************************************
 * En este archivo se encuentran definidas las funciones que se implementarán en el codigo           *
 *                                                                                                   *
 *****************************************************************************************************/

/************************************************* Headers **************************************************/
#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stddef.h>

/* ---- Prototipos ---- */
int *leer_enteros(char *ruta, size_t n);
int   sumar_arreglo(int *v, size_t n);

#endif
