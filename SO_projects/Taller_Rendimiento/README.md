# **Evaluación de Rendimiento: Multiplicación de Matrices Paralela**

Este proyecto implementa diferentes versiones del algoritmo de multiplicación de matrices cuadradas y compara su rendimiento mediante técnicas de paralelismo: **Fork**, **POSIX Threads**, **OpenMP** y **OpenMP por Filas**.

---

El proyecto incluye las siguientes versiones del algoritmo:

* **mmClasicaFork** → Paralelismo basado en procesos mediante `fork()`.
* **mmClasicaPosix** → Paralelismo por hilos POSIX (`pthread_create`).
* **mmClasicaOpenMP** → Paralelismo con OpenMP utilizando secciones paralelas estándar.
* **mmClasicaOpenMP_Filas** → Variante OpenMP que distribuye trabajo por filas.

Cada versión mide el tiempo de ejecución usando `gettimeofday()`.

### **Tamaños de Matriz Seleccionados:**

Se escogieron las dimensiones **2, 3 y 5** porque:

* Son matrices **muy pequeñas**, lo que permite observar cómo se comporta cada técnica en escenarios donde el costo de paralelizar puede superar el costo de calcular.
* Permiten detectar **sobrecargas**, picos anómalos y escalabilidad negativa.
* Facilitan la identificación clara de patrones en las gráficas.

---

### **Cantidad de Hilos Evaluada**

Se utilizaron **1, 2, 4 y 8 hilos**, escogidos porque:

* **1 hilo** → línea base (ejecución secuencial).
* **2 hilos** → primera división del trabajo.
* **4 hilos** → coincide con el número de cores físicos de la CPU.
* **8 hilos** → permite observar **sobresaturación**, ya que excede la capacidad del hardware.

### **Automatización de las Pruebas**

El archivo `lanzador.pl` ejecuta automáticamente cada combinación:

```perl
$Nombre_Ejecutable = "mmClasicaFork";
@Size_Matriz = ("2","3","5");
@Num_Hilos = (1,2,4,8);
$Repeticiones = 30;
```

Para cada caso crea archivos:

```
mmClasicaFork-2-Hilos-1.dat
mmClasicaFork-2-Hilos-2.dat
...
mmClasicaFork-5-Hilos-8.dat
```

Cada archivo contiene **30 mediciones independientes**, útiles para análisis estadístico.

---

## **Análisis Detallado Basado Exclusivamente en las Gráficas**

### **Comparación entre tamaños**

En todas las técnicas:

* **Tamaño 2** es siempre el más rápido.
* **Tamaño 3** ocupa la posición intermedia.
* **Tamaño 5** es el más lento.

#### **Fork**

* Aumentar hilos aumenta el tiempo sin excepción.
* No presenta comportamientos irregulares.
* Escalabilidad negativa para matrices pequeñas.

#### **POSIX Threads**

* Tiempos más bajos que Fork en todos los casos.
* Crecimiento estable y sin picos inesperados.

#### **OpenMP**

* Muestra mayor variabilidad.
* 4 hilos produce el peor desempeño en todas las dimensiones.
* 8 hilos reduce el tiempo respecto a 4, pero no alcanza a 1 hilo.

#### **OpenMP (Filas)**

* Tendencia muy similar a OpenMP estándar.
* El peor caso aparece en 4 hilos.
* Se presenta una caída en 8 hilos, aunque sigue siendo inestable.

## parseResultados.c — Conversión a CSV

Este archivo toma todos los archivos .dat y genera un CSV con la siguiente estructura:

Algoritmo;TamMatriz;Hilos;Repeticion;Tiempo_microsegundos

Lee el nombre del archivo
Extrae:

-Algoritmo (ej. mmClasicaFork) 
-Tamaño de la matriz 
-Número de hilos 

Ejemplo:
mmClasicaFork-5-Hilos-8.dat → 5, 8.

Abre el archivo .dat y busca líneas con:
"Tiempo total de ejecución"
Por cada coincidencia escribe una fila en el CSV.

Cómo compilar:
```
gcc -Wall -Wextra -std=c11 parseResultados.c -o parseResultados
```

Cómo usar:

Ejemplo para procesar todos los .dat:

```
./parseResultados resultados.csv mmClasicaFork-*.dat
```

