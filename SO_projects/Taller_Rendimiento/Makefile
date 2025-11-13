#===============================================================================
#  Makefile para la compilación de programas de Multiplicación de Matrices
#  Autor: J. Corredor, PhD (adaptado y organizado por Carolina Ujueta Ricardo)
#  Fecha: 13 Noviembre 2025
#===============================================================================

# Compilador y banderas
GCC = gcc
CFLAGS = -Wall -lm
FOPENMP = -fopenmp -O3
POSIX = -lpthread

# Programas principales (ejecutables)
PROGRAMAS = mmClasicaFork mmClasicaOpenMP mmClasicaPosix mmFilasOpenMP

# Regla general
all: $(PROGRAMAS)

# Compilaciones específicas
mmClasicaFork: mmClasicaFork_main.c
	$(GCC) $(CFLAGS) $< -o $@ $(POSIX)

mmClasicaOpenMP: mmClasicaOpenMP_main.c
	$(GCC) $(CFLAGS) $< -o $@ $(FOPENMP)

mmClasicaPosix: mmClasicaPosix_main.c
	$(GCC) $(CFLAGS) $< -o $@ $(POSIX)

mmFilasOpenMP: mmFilasOpenMP_main.c
	$(GCC) $(CFLAGS) $< -o $@ $(FOPENMP)

# Limpieza
clean:
	rm -f $(PROGRAMAS)
