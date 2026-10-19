# P0_T9 — Máximo de cada columna mediante submatrices

## Enunciado

Haz un programa en C (no C++) que lea del fichero binario `fichero2.bin` una matriz cuadrada `MatTotal` de tipo `unsigned char` y dimensiones **512 × 512**.

La matriz `MatTotal` deberá almacenarse en memoria dinámica mediante un puntero doble.

El programa recibirá, mediante un argumento de la línea de ejecución, el número de submatrices en las que se desea dividir `MatTotal`. Este número deberá estar comprendido entre **2 y 5**, ambos inclusive.

La matriz se dividirá en **bloques de columnas consecutivas**, de forma que todas las columnas de `MatTotal` queden distribuidas entre las submatrices.

Las submatrices deberán tener un número de columnas igual o lo más parecido posible. Todas tendrán siempre **512 filas**.

Reserva dinámicamente las submatrices necesarias y copia en ellas, en el mismo orden, todas las columnas de `MatTotal`.

Una vez realizada la copia, libera completamente la memoria ocupada por `MatTotal`. A partir de ese momento no se podrá acceder a dicha matriz.

Reserva dinámicamente un vector `MaxColumna` de **512 elementos** de tipo `unsigned char`.

Utilizando únicamente las submatrices creadas, calcula el valor máximo de cada una de las 512 columnas de la matriz original y almacénalo en `MaxColumna`, de forma que `MaxColumna[j]` contenga el máximo correspondiente a la columna `j` de `MatTotal`.

## Importante

La copia de los datos desde `MatTotal` a las submatrices deberá realizarse utilizando la función `memcpy`.

Se deberá utilizar el menor número posible de llamadas a `memcpy`, teniendo en cuenta la forma en la que están almacenados los datos en memoria y el tipo de división realizada.

No se permite realizar la copia elemento a elemento mediante bucles.
