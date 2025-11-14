##############################################################################################################
#                                                                                                            #
#            Pontificia Universidad Javeriana                                                                #
#                                                                                                            #
# Autor:     Thomas Leal Puerta, Carolina Ujueta Ricardo                                                     #
# Fecha:     13 de Noviembre de 2025                                                                         #
# Materia:   Sistemas Operativos                                                                             #
# Profesor:  John Jairo Corredor, PhD                                                                        #
# Objetivo:  Automatizar la compilación de taller Posix consumer.c producer.c utilizando GCC                 #
#                                                                                                            #
##############################################################################################################
#                                                                                                            #
# HOW TO USE                                                                                                 #
# ---------------------------------------------------------------------------------------------------------- #
# Compilar todos los laboratorios  : make                                                                    #
# Compilar un laboratorio especifico: make producer                                                          #
# Limpiar ejecutables generados    : make clean                                                              #
#                                                                                                            #
# Descripción:                                                                                               #
#                                                                                                            #
#   Este Makefile genera los binarios de los laboratorios de Ficheros.                                       #
#   Cada regla compila un archivo fuente .c con lectura de ficheros.                                         #
#                                                                                                            #
##############################################################################################################

# ************************************************* Variables *************************************************

GCC = gcc
FLAGS =
PROGRAMAS = producer consumer

# ************************************************* Reglas ****************************************************

# ---- Regla por defecto: compila todos los programas listados ----
all: $(PROGRAMAS)

# ---- Compilación del laboratorio 1 ----
producer: producer.c
        $(GCC) $@.c -o $@ $(FLAGS)

# ---- Compilación del laboratorio 2 ----
consumer: consumer.c
        $(GCC) $@.c -o $@ $(FLAGS)

# ---- Elimina todos los ejecutables generados ----
clean:
        rm -f $(PROGRAMAS)
