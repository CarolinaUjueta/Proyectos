##############################################################################################################
#            Pontificia Universidad Javeriana                                                                #
# Autor:     Carolina Ujueta Ricardo                                                                         #
# Fecha:     14 de Noviembre de 2025                                                                         #
# Materia:   Sistemas Operativos                                                                             #
# Objetivo:  Compilar las variantes de Multiplicación de Matrices (Fork, OpenMP, POSIX, OpenMP-filas)        #
##############################################################################################################
# HOW TO USE                                                                                                 #
#   Compilar todos los programas  : make                                                                     #
#   Compilar uno específico       : make mmClasicaFork                                                       #
#   Limpiar ejecutables generados : make clean                                                               #
##############################################################################################################

# *********************** Variables ***********************

GCC      = gcc
FLAGS    = -lm -fopenmp
PROGRAMAS = mmClasicaFork mmClasicaOpenMP mmClasicaPosix mmFilasOpenMP

# *********************** Reglas **************************

# Regla por defecto: compila todos los programas
all: $(PROGRAMAS)

# ---- Fork ----
mmClasicaFork: Fork/mmClasicaFork_main.c Fork/mmClasicaFork.c
	$(GCC) Fork/mmClasicaFork_main.c Fork/mmClasicaFork.c -o $@ $(FLAGS)

# ---- OpenMP (clásica) ----
mmClasicaOpenMP: OpenMP/mmClasicaOpenMP_main.c OpenMP/mmClasicaOpenMP.c
	$(GCC) OpenMP/mmClasicaOpenMP_main.c OpenMP/mmClasicaOpenMP.c -o $@ $(FLAGS)

# ---- POSIX ----
mmClasicaPosix: Posix/mmClasicaPosix.c
	$(GCC) Posix/mmClasicaPosix.c -o $@ $(FLAGS)

# ---- OpenMP por filas ----
mmFilasOpenMP: OpenMP-filas/mmFilasOpenMP_main.c OpenMP-filas/mmFilesOpenMP.c
	$(GCC) OpenMP-filas/mmFilasOpenMP_main.c OpenMP-filas/mmFilesOpenMP.c -o $@ $(FLAGS)

# ---- Limpiar ejecutables ----
clean:
	rm -f $(PROGRAMAS)
