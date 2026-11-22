# Tarea 3

Se debe desarrollar un código paralelo usando **MPI** que procese una matriz de tamaño **n filas × m columnas**, con ambos valores pasados por parámetro.

La matriz la genera el **proceso 0**, considerando que se referencia con un **puntero doble** y, por tanto, no están todos los datos contiguos en memoria. Los datos son de tipo **entero**.

El resultado es un **vector que almacena la media de todas y cada una de las filas**. El **proceso 0** debe guardar dicho vector en un **fichero binario**.
