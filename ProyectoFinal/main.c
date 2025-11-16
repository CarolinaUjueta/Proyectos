/* -------------------------------------------------------------------------- */
/*                           Funcion principal del agente                      */
/* -------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <NombreAgente> <ArchivoSolicitudes>\n", argv[0]);
        return 1;
    }

    char nombre_agente[MAX_LONG_NOMBRE_AGENTE];
    char archivo_entrada[128];
    char pipe_respuesta[MAX_LONG_NOMBRE_PIPE];

    strncpy(nombre_agente, argv[1], MAX_LONG_NOMBRE_AGENTE);
    strncpy(archivo_entrada, argv[2], sizeof(archivo_entrada));

    snprintf(pipe_respuesta, sizeof(pipe_respuesta),
             "pipe_respuesta_%s", nombre_agente);

    /* Crear FIFO del agente */
    if (mkfifo(pipe_respuesta, 0666) < 0) {
        perror("Error creando pipe de respuesta");
        return 1;
    }

    printf("[AGENTE %s] Pipe de respuesta creado: %s\n",
           nombre_agente, pipe_respuesta);

    /* Registrar agente */
    if ( registrar_agente(nombre_agente, pipe_respuesta) < 0 ) {
        fprintf(stderr, "No se pudo registrar el agente.\n");
        unlink(pipe_respuesta);
        return 1;
    }

    printf("[AGENTE %s] Registro enviado al controlador.\n", nombre_agente);

    /* ----------------------
       Apertura del archivo
       ---------------------- */
    FILE *f = fopen(archivo_entrada, "r");
    if (!f) {
        perror("No se pudo abrir archivo de entrada");
        unlink(pipe_respuesta);
        return 1;
    }

    char linea[MAX_LONG_LINEA];
    solicitud_reserva_t sol;
    respuesta_reserva_t resp;

    /* Preparar campos constantes de la solicitud */
    strcpy(sol.nombre_agente, nombre_agente);
    strcpy(sol.pipe_respuesta, pipe_respuesta);

    /* ----------------------
       LECTURA LINEA A LINEA
       ---------------------- */
    while (fgets(linea, sizeof(linea), f)) {

        /* Formato típico: Familia Hora Personas */
        char familia[MAX_LONG_NOMBRE_FAMILIA];
        int  hora, num;

        if (sscanf(linea, "%s %d %d", familia, &hora, &num) != 3) {
            continue;  /* línea incorrecta */
        }

        strcpy(sol.nombre_familia, familia);
        sol.hora_solicitada = hora;
        sol.num_personas    = num;

        /* Enviar solicitud y esperar respuesta */
        if (enviar_solicitud_y_recibir(nombre_agente,
                                       pipe_respuesta,
                                       &sol,
                                       &resp) < 0) {
            fprintf(stderr, "[AGENTE %s] Error en comunicación.\n", nombre_agente);
            break;
        }

        /* ------------------------------------------
           IMPRIMIR RESPUESTA DEL CONTROLADOR
           ------------------------------------------ */
        switch (resp.tipo) {

        case RESPUESTA_RESERVA_OK:
            printf("[AGENTE %s] Aprobada: Familia %s Hora %d Personas %d\n",
                   nombre_agente,
                   resp.reserva.nombre_familia,
                   resp.reserva.hora_inicio,
                   resp.reserva.num_personas);
            break;

        case RESPUESTA_RESERVA_REPROGRAMADA:
            printf("[AGENTE %s] Reprogramada: Familia %s NuevaHora %d\n",
                   nombre_agente,
                   resp.reserva.nombre_familia,
                   resp.reserva.hora_inicio);
            break;

        case RESPUESTA_RESERVA_NEGADA_EXTEMP:
        case RESPUESTA_RESERVA_NEGADA_SIN_CUPO:
        case RESPUESTA_RESERVA_NEGADA_AFORO:
            printf("[AGENTE %s] Negada para familia %s (%s)\n",
                   nombre_agente,
                   resp.reserva.nombre_familia,
                   resp.mensaje);
            break;
        }

        sleep(2);
    }

    fclose(f);
    unlink(pipe_respuesta);

    printf("Agente %s termina.\n", nombre_agente);
    return 0;
}
