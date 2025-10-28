/*********************************************************************************************
 * Pontificia Universidad Javeriana
 *
 * Autor:       Carolina Ujueta Ricardo
 * Materia:     Sistemas Operativos
 * Docente:     J. Corredor, PhD
 * Fecha:       27/10/2025
 * Tema:        Introducción a hilos POSIX (pthread)
 *
 * Descripción general:
 *     Programa en C que demuestra la creación y ejecución concurrente de hilos
 *     utilizando la biblioteca pthread. Cada hilo imprime un mensaje independiente,
 *     mostrando la ejecución simultánea de tareas. El ejercicio permite familiarizarse
 *     con la sintaxis básica de pthread, la creación de hilos con pthread_create,
 *     y la sincronización mediante pthread_join.
 *********************************************************************************************/

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

/*
 ============================================================================
 Función: print_message_function
 Descripción:
    Función que ejecutará cada hilo creado.
    Recibe un puntero genérico (void*) que se interpreta como puntero a cadena de caracteres.
    Imprime el mensaje pasado como argumento, simulando tareas independientes ejecutadas
    de forma concurrente.
 ============================================================================
*/
void *print_message_function(void *ptr) {
    char *message;
    message = (char *)ptr; // Conversión del puntero genérico a cadena
    printf("%s\n", message);
    return NULL; // Devuelve NULL explícitamente por buenas prácticas
}

/*
 ============================================================================
 Función principal: main
 Descripción:
    Punto de entrada del programa.
    Crea dos hilos que ejecutan la misma función con distintos mensajes.
    Se valida el resultado de creación de cada hilo y se sincroniza su finalización
    mediante pthread_join, asegurando que ambos terminen antes de finalizar el programa.
 ============================================================================
*/
int main(void) {
    pthread_t thread1, thread2;  // Identificadores de los hilos
    char *message1 = "Thread 1"; // Mensaje del primer hilo
    char *message2 = "Thread 2"; // Mensaje del segundo hilo
    int iret1, iret2;            // Códigos de retorno de creación

    /*
     * Creación de los hilos independientes.
     * Cada uno ejecutará la misma función pero con mensajes distintos.
     */
    iret1 = pthread_create(&thread1, NULL, print_message_function, (void*) message1);
    iret2 = pthread_create(&thread2, NULL, print_message_function, (void*) message2);

    /*
     * Comprobación de resultados:
     * pthread_create devuelve 0 si el hilo se creó correctamente.
     */
    printf("Thread 1 returns: %d\n", iret1);
    printf("Thread 2 returns: %d\n", iret2);

    /*
     * Sincronización de los hilos.
     * pthread_join garantiza que ambos hilos terminen su ejecución
     * antes de que el programa principal finalice.
     */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Finalización correcta del programa.\n");

    return 0; // Finalización exitosa del programa principal
}
