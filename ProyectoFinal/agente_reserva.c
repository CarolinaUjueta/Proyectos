/********************************************************************************************************
*                                       Pontificia Universidad Javeriana                                *
* Autor:      Carolina Ujueta Ricardo                                                                   *
* Fecha:      17 noviembre, 2025                                                                        *
* Materia:    Sistemas Operativos                                                                       *
* Archivo:    agente_reserva.c                                                                          *
* Descripción: Programa Agente que se conecta con el Controlador, se registra y luego envía solicitudes *
*********************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "controlador_reserva.h"

#define MAX_LONG_LINEA 256

#define FIFO_CONTROLADOR "pipecontrol"

/* -------------------------------------------------------------------------- */
/* Función que envía una SOLICITUD y espera la RESPUESTA del Controlador     */
/* -------------------------------------------------------------------------- */
static int enviar_solicitud_y_recibir(
        const char *nombre_agente,
        const char *pipe_respuesta,
        solicitud_reserva_t *sol,
        respuesta_reserva_t *resp)
{
    int fd_ctrl, fd_resp;
    char linea[MAX_LONG_LINEA];

    /* Abrimos FIFO del controlador */
    fd_ctrl = open(FIFO_CONTROLADOR, O_WRONLY);
    if (fd_ctrl < 0) {
        perror("Error abriendo FIFO del controlador");
        return -1;
    }

    /* Serializamos la solicitud.
       Formato definido por el grupo:
       SOLICITUD;<agente>;<familia>;<hora>;<personas>;<pipeRes>
    */
    snprintf(linea, sizeof(linea),
             "SOLICITUD;%s;%s;%d;%d;%s\n",
             sol->nombre_agente,
             sol->nombre_familia,
             sol->hora_solicitada,
             sol->num_personas,
             sol->pipe_respuesta);

    write(fd_ctrl, linea, strlen(linea));
    close(fd_ctrl);

    /* -----------------------------------------------------------
       Ahora esperamos respuesta del Controlador por el pipe propio
       ----------------------------------------------------------- */
    fd_resp = open(pipe_respuesta, O_RDONLY);
    if (fd_resp < 0) {
        perror("Error abriendo pipe de respuesta");
        return -1;
    }

    /* La respuesta llega como una estructura binaria */
    ssize_t n = read(fd_resp, resp, sizeof(*resp));
    close(fd_resp);

    if (n != sizeof(*resp)) {
        fprintf(stderr, "Error: respuesta inválida.\n");
        return -1;
    }

    return 0;
}
