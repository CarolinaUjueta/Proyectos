################################################################################
#  Makefile para compilar los programas de laboratorio OpenMP
#  Autor:       Carolina Ujueta Ricardo
#  Fecha:       4 de noviembre de 2025
#  Descripción: Compila, ejecuta y limpia los ejecutables de distintos programas
#               que implementan directivas OpenMP, paralelización y medición
#               de rendimiento.
################################################################################

# Compilador y banderas
CC = gcc
CFLAGS = -Wall -fopenmp -O2
LDFLAGS = -lm

# Archivos fuente
SRC = lab01.c lab02.c lab03.c lab04.c Sumatoria_seno.c
# Ejecutables generados
TARGETS = lab01 lab02 lab03 lab04 Sumatoria_seno

# Regla general de compilación
all: $(TARGETS)

# Compilación individual para cada fuente
lab01: lab01.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

lab02: lab02.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

lab03: lab03.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

lab04: lab04.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

Sumatoria_seno: Sumatoria_seno.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Ejecutar programa por defecto (ejemplo)
run: Sumatoria_seno
	./Sumatoria_seno 4 30000

# Ejecutar con parámetros personalizados
run_custom: Sumatoria_seno
	./Sumatoria_seno $(ARGS)

# Limpieza de binarios
clean:
	rm -f $(TARGETS)
