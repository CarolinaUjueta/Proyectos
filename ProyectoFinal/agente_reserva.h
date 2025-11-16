#ifndef AGENTE_RESERVA_H
#define AGENTE_RESERVA_H

#define MAXLINE 256

/* Funciones del agente */
int registrar_agente(const char *nombre, const char *pipe_srv, const char *pipe_resp);
int enviar_solicitud(const char *familia, int personas, int hora_inicio,
                     const char *pipe_srv, const char *pipe_resp);

#endif
