/****************************************************************************************************
*                               Pontificia Universidad Javeriana                                    *
*                                                                                                   *
* Autor:     Thomas Leal PUerta, Carolina Ujueta Ricardo                                            *
* Fecha:     13/11/2025                                                                             *
* Materia:   Sistemas Operativos                                                                    *
* Profesor:  John Jairo Corredor Franco                                                             *
*  Objetivo:     Implementar el proceso CONSUMIDOR del problema Productor–Consumidor, utilizando    *
*                semaforos POSIX con nombre para sincronizar y memoria compartida para intercambiar *
*                datos con el productor.                                                            *
*                                                                                                   *
* Descripcion:                                                                                      *
* Este programa actúa como CONSUMIDOR. Se conecta a los semáforos y a la memoria compartida que     *
* el productor creó previamente, y extrae 10 elementos del búfer circular compartido.               *
*                                                                                                   *
* Elementos clave:                                                                                  *
*   • Usa semáforos con nombre:                                                                     *
*         - /vacio : indica que hay espacio libre en el búfer (lo incrementa el consumidor).        *
*         - /lleno : indica que hay elementos disponibles (lo decrementa el consumidor).            *
*                                                                                                   *
*   • Usa un segmento de memoria compartida que contiene:                                           *
*         - Un arreglo circular de enteros (bus[])                                                  *
*         - El índice de lectura (salida)                                                           *
*****************************************************************************************************
*                                                                                                   *
*                                   HOW TO USE / COMPILE / RUN                                      *
*  COMPILAR:   gcc consumer.c -o consumer                                                           *
*  EJECUTAR:   ./consumer                                                                           *
*  NOTAS:      - Espera a que el productor cree los semaforos y la memoria compartida.              *
*              - Consume 10 elementos del bufer circular compartido.                                *
*                                                                                                   *
*****************************************************************************************************/

/* ============================================== Headers ========================================== */
#include "semaforo.h"

/****************************************************************************************************
*                                                                                                   *
*  Funcion:      int main()                                                                         *
*  Proposito:    Abrir los semaforos y el segmento de memoria compartida creados por el productor,  *
*                y consumir 10 elementos del bufer circular respetando la sincronizacion.           *
*                                                                                                   *
*****************************************************************************************************/
int main() {
    /* ---- Abrir semaforos con nombre (deben existir) ---- */
    sem_t *vacio = sem_open("/vacio", 0);
    sem_t *lleno = sem_open("/lleno", 0);
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    /* ---- Abrir el objeto de memoria compartida existente ---- */
    int fd_compartido = shm_open("/memoria_compartida", O_RDWR, 0644);
    if (fd_compartido < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    /* ---- Mapear el segmento para acceder al bufer compartido ---- */
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, fd_compartido, 0);
    if (compartir == MAP_FAILED) {
        perror("mmap");
        close(fd_compartido);
        exit(EXIT_FAILURE);
    }

    /* ---- Inicializar indice de lectura (salida) del bufer circular ---- */
    compartir->salida = 0;

    /* ---- Consumir 10 elementos, respetando la disponibilidad de datos ---- */
    for (int i = 1; i <= 10; i++) {
        /* ---- Esperar a que haya al menos un elemento disponible ---- */
        sem_wait(lleno);

        /* ---- Leer elemento y avanzar indice circular ---- */
        int item = compartir->bus[compartir->salida];
        printf("Consumidor: Consume %d\n", item);
        compartir->salida = (compartir->salida + 1) % BUFFER;

        /* ---- Sennalar que quedo un espacio libre ---- */
        sem_post(vacio);

        /* ---- Ritmo de consumo (simulacion) ---- */
        sleep(2);
    }

    /* ---- Desmapear memoria y liberar descriptores locales ---- */
    munmap(compartir, sizeof(compartir_datos));
    close(fd_compartido);

    /* ---- Cerrar semaforos locales (ver notas de limpieza de nombres) ---- */
    sem_close(vacio);
    sem_close(lleno);

    /* ---- de lo contrario, comentalas para que no se eliminen los objetos antes de tiempo. ---- */
    sem_unlink("/lleno");
    shm_unlink("/memoria_compartida"); 

    return 0;
}
