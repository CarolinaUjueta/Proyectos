/********************************************************************************************************
*                                       Pontificia Universidad Javeriana                                *
* Autor:      Carolina Ujueta Ricardo                                                                   *
* Fecha:      17 noviembre, 2025                                                                        *
* Materia:    Sistemas Operativos                                                                       *
* Archivo:    agente_reserva.c                                                                          *
* Descripción: Programa Agente que se conecta con el Controlador, se registra y luego envía solicitudes *
*********************************************************************************************************/

#include "agente_reserva.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* --------------------------------------------------------------
   Enviar registro inicial al Controlador
   --------------------------------------------------------------*/
int registrar_agente(const char *nombre, const char *pipe_srv, const char *pipe_resp)
{
    int fd;
    char msg[MAXLINE];

    fd = open(pipe_srv, O_WRONLY);
    if (fd < 0) {
        perror("open pipe controlador");
        return -1;
    }

    snprintf(msg, sizeof(msg), "REGISTRO;%s;%s\n", nombre, pipe_resp);
    write(fd, msg, strlen(msg));
    close(fd);

    return 0;
}

/* --------------------------------------------------------------
   Enviar solicitud normal
   --------------------------------------------------------------*/
int enviar_solicitud(const char *familia, int personas, int hora_inicio,
                     const char *pipe_srv, const char *pipe_resp)
{
    int fd;
    char msg[MAXLINE];

    fd = open(pipe_srv, O_WRONLY);
    if (fd < 0) {
        perror("open pipe controlador");
        return -1;
    }

    int hora_fin = hora_inicio + 2;

    snprintf(msg, sizeof(msg),
             "SOLICITUD;%s;%d;%d;%d;%s\n",
             familia, personas, hora_inicio, hora_fin, pipe_resp);

    write(fd, msg, strlen(msg));
    close(fd);

    return 0;
}
