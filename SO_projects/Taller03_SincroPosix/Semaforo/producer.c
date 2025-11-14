/****************************************************************************************************
*                                  PONTIFICIA UNIVERSIDAD JAVERIANA                                 *
*                                       Departamento de Sistemas                                    *
*                                       SISTEMAS OPERATIVOS                                         *
*                                                                                                   *
*  Autor:        Thomas Leal Puerta, Carolina Ujueta Ricardo                                        *
*  Fecha:        2025-11-09                                                                         *
*  Materia:      Sistemas Operativos                                                                *
*  Profesor:     Dr. John Jairo Corredor                                                            *
*  Objetivo:     Implementar el proceso PRODUCTOR del problema Productor–Consumidor empleando       *
*                semaforos POSIX con nombre y memoria compartida (POSIX shm + mmap).                *
*****************************************************************************************************
*                                   HOW TO USE / COMPILE / RUN                                      *
*  COMPILAR:   gcc -o producer producer.c -lrt -pthread                                             *
*  EJECUTAR:   ./producer                                                                           *
*  NOTAS:      - Requiere que el encabezado compartido defina: BUFFER y la struct compartir_datos.  *
*              - Este proceso crea/ajusta la memoria compartida y los semaforos con nombre.         *
*              - Empuja 10 elementos al bufer circular.                                             *
*****************************************************************************************************/

/*============================================== Headers ==========================================*/

#include "semaforo.h"

/****************************************************************************************************
*  Funcion:      int main()                                                                         *
*  Proposito:    Crear/abrir los semaforos con nombre y la memoria compartida; producir 10 elementos*
*                en el bufer circular, sincronizando con semaforos vacio (espacios libres) y        *
*                lleno (espacios ocupados).                                                         *
*  Parametros:   (ninguno)                                                                          *
*  Retorno:      1 si no hay errores; EXIT_FAILURE en caso de fallo.                                *
*****************************************************************************************************/
int main() {
    /* ---- Abrir/crear semaforos con nombre: vacio y lleno ---- */
    sem_t *vacio = sem_open("/vacio", O_CREAT, 0644, BUFFER);
    sem_t *lleno = sem_open("/lleno", O_CREAT, 0644, 0);
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    /* ---- Crear/abrir objeto de memoria compartida ---- */
    int shm_fd = shm_open("/memoria_compartida", O_CREAT | O_RDWR, 0644);
    if (shm_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    /* ---- Ajustar tamaño del segmento compartido a la estructura requerida ---- */
    if (ftruncate(shm_fd, sizeof(compartir_datos)) == -1) {
        perror("ftruncate");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    /* ---- Mapear el objeto de memoria compartida en el espacio del proceso ---- */
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (compartir == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    /* ---- Inicializar indice de escritura (entrada) del bufer circular ---- */
    compartir->entrada = 0;

    /* ---- Producir 10 elementos, respetando la disponibilidad de espacio ---- */
    for (int i = 1; i <= 10; i++) {
        /* ---- Esperar espacio libre (P en vacio) ---- */
        sem_wait(vacio);

        /* ---- Escribir elemento y avanzar indice circular ---- */
        compartir->bus[compartir->entrada] = i;
        printf("Productor: Produce %d\n", i);
        compartir->entrada = (compartir->entrada + 1) % BUFFER;

        /* ---- Señalar que hay un elemento mas disponible (V en lleno) ---- */
        sem_post(lleno);

        /* ---- Ritmo de produccion (simulacion) ---- */
        sleep(1);
    }

    /* ---- Desmapear memoria y liberar descriptores locales ---- */
    munmap(compartir, sizeof(compartir_datos));
    close(shm_fd);

    /* ---- Cerrar semaforos locales (eliminar nombres en este proceso si asi se desea) ---- */
    sem_close(vacio);
    sem_close(lleno);

    /* ----      de lo contrario, comenta unlink y haz limpieza al final del flujo de pruebas. ---- */
    sem_unlink("/vacio");
    sem_unlink("/lleno");
    shm_unlink("/memoria_compartida");

    return 0;
}
