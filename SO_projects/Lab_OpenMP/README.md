# Programa procesos en C

Este proyecto implementa un conjunto de programas en C de manera modular, aplicando conceptos de **paralelismo con OpenMP**, **manejo de variables privadas y compartidas**, **reducción**, y **medición de tiempo de ejecución**.  
Cada laboratorio aborda un aspecto distinto del uso de OpenMP y la programación concurrente.

## Estructura del proyecto

- lab01.c: Ejemplo inicial de paralelismo con OpenMP, muestra el uso básico de `#pragma omp parallel`.
- lab02.c: Introduce el uso de variables privadas y compartidas dentro de regiones paralelas.
- lab03.c: Explora el efecto de las directivas `private()` y `shared()` en ciclos `for` paralelos.
- lab04.c: Extiende los conceptos de control de variables con OpenMP, analizando condiciones de carrera.
- Sumatoria_seno.c: Implementa un cálculo paralelo de sumatoria usando una función trigonométrica y medición de tiempo.

- Makefile: Automatiza la compilación, ejecución y limpieza de todos los binarios.
- README.md: Documentación general del proyecto y sus componentes.

## Uso del Makefile

- Compilar todo: make
- Ejecutar for 3000: make run
- Ejecutar personalizado: make run_custom ARGS="8 60000"
- Ejecutar: ./programa (lab0*.c o Sumatoria_seno.c)
- Limpiar ejecutables: make clean
