# TAREA 1

Se debe desarrollar un código paralelo usando MPI que procese un vector de dimensión **6000** de números enteros, con valores entre **-10000 y 10000**.

El **proceso 0** generará aleatoriamente el vector, asegurándose de que haya **al menos 100 valores iguales a 0**.

Hay que calcular en paralelo:

- La cantidad de números impares.
- La cantidad de números pares distintos de 0.
- La cantidad de números cuyas cifras suman 19.

**Solo se utilizarán comunicaciones punto a punto.**
