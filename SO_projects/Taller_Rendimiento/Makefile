#===============================================================================
#  Makefile para la compilación de programas de Multiplicación de Matrices
#  Autor: J. Corredor, PhD (adaptado y organizado por Carolina Ujueta Ricardo)
#  Fecha: 13 Noviembre 2025
#===============================================================================

# Compilador y banderas
CC = gcc
CFLAGS = -Wall -lm
FOPENMP = -fopenmp -O3
POSIX = -lpthread

# Programas (uno por carpeta)
PROGRAMAS = mmClasicaFork mmClasicaOpenMP mmClasicaPosix mmFilasOpenMP

# Regla principal
all: $(PROGRAMAS)

mmClasicaFork:
	$(CC) $(CFLAGS) $(POSIX) Fork/mmClasicaFork_main.c -o $@

mmClasicaOpenMP:
	$(CC) $(CFLAGS) $(FOPENMP) OpenMP/mmClasicaOpenMP_main.c -o $@

mmClasicaPosix:
	$(CC) $(CFLAGS) $(POSIX) Posix/mmClasicaPosix_main.c -o $@

mmFilasOpenMP:
	$(CC) $(CFLAGS) $(FOPENMP) OpenMP-filas/mmFilasOpenMP_main.c -o $@

clean:
	rm -f $(PROGRAMAS)

