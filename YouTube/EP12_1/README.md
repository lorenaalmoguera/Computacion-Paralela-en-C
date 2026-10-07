# P0_T4 — Máximo por partes de un vector

## Enunciado

Se dispone de un fichero binario que contiene datos almacenados consecutivamente. El tamaño de ese fichero es de **4000 bytes**. El programa deberá trabajar con un solo vector de **1000 enteros**, `V1`.

El número de partes en las que se debe dividir (lógicamente) el vector se indicará mediante un argumento en la línea de ejecución del programa. Dicho número será como máximo **10**.

La suma de los tamaños del número de partes indicado coincide exactamente con los **1000 elementos**.

El objetivo será dividir lógicamente el vector en partes de igual (o casi igual) tamaño y calcular el valor máximo de cada parte, trabajando siempre sobre el mismo vector original y sin realizar copias de sus elementos.

Para cada parte se deberá mostrar:

- El índice de comienzo dentro de `V1` (la primera parte comienza en la posición 0).
- El número de elementos que contiene.
- El valor máximo de dicha parte.

Finalmente, utilizando los máximos obtenidos para cada parte, se calculará el máximo global del vector, es decir, un único valor correspondiente al mayor de los 1000 enteros almacenados en `V1`.
