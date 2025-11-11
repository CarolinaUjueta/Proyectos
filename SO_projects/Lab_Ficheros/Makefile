###############################################################>
#  Makefile para compilar los programas de laboratorio ficheros
#  Autor:       Carolina Ujueta Ricardo
#  Fecha:       11 de noviembre de 2025
#  Descripción: Compila, ejecuta y limpia los ejecutables de
#               Labs ficheros
###############################################################>

# Compilador y banderas
CC = gcc
FLAGS = -Wall

# Archivos fuente
SRC = lab01.c lab02.c
# Ejecutables generados
TARGETS = lab01 lab02

# Regla general de compilación
all: $(TARGETS)

# Compilación individual para cada fuente
lab01: lab01.c
        $(CC) $(FLAGS) $< -o $@

lab02: lab02.c
        $(CC) $(FLAGS) $< -o $@

# Limpieza de binarios
clean:
        rm -f $(TARGETS)
