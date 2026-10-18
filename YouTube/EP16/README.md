# P0_T8 - Máximo de cada columna mediante submatrices

## Enunciado

Haz un programa en C (no C++) que lea desde el fichero binario `fichero2.bin` una matriz cuadrada `MatTotal` de tipo `unsigned char` y dimensiones **512 × 512**.

La matriz `MatTotal` deberá almacenarse utilizando memoria dinámica mediante un puntero doble.

El programa recibirá mediante un argumento de la línea de ejecución el **número de submatrices** en las que se desea dividir `MatTotal`. Este número deberá estar comprendido entre **2 y 5**, ambos inclusive.

La matriz se dividirá en bloques de filas consecutivas, de forma que todas las filas de `MatTotal` queden distribuidas entre las submatrices.

Las submatrices deberán tener un número de filas igual o lo más parecido posible. En caso de que 512 no sea divisible exactamente entre el número de submatrices, las primeras submatrices tendrán una fila más que las restantes.

Cada submatriz tendrá siempre **512 columnas**.

Por ejemplo (este cálculo hay que realizarlo por código):

- Si se indican 2 submatrices, cada una tendrá 256 filas.
- Si se indican 3 submatrices, tendrán 171, 171 y 170 filas.

Reserva dinámicamente las submatrices necesarias y copia en ellas, en el mismo orden, todas las filas de `MatTotal`.

Una vez realizada la copia, libera completamente la memoria ocupada por `MatTotal`. A partir de ese momento no se podrá acceder a dicha matriz.

Reserva dinámicamente un vector `MaxColumna` de **512 elementos** de tipo `unsigned char`.

Utilizando únicamente las submatrices creadas, calcula el valor máximo de cada una de las 512 columnas de la matriz original y almacénalo en `MaxColumna`, de forma que `MaxColumna[j]` contenga el máximo correspondiente a la columna `j` de `MatTotal`.

Para calcular el máximo de una columna será necesario recorrer dicha columna a través de todas las submatrices.

Finalmente, muestra por pantalla, para cada columna:

- El número de columna.
- El valor máximo de dicha columna.

Antes de finalizar el programa deberán liberarse correctamente todas las zonas de memoria dinámica utilizadas.

## Importante

La copia de los datos desde `MatTotal` a las submatrices deberá realizarse utilizando la función `memcpy`.

Se deberá utilizar el menor número posible de llamadas a `memcpy`, teniendo en cuenta la forma en la que están almacenados los datos en memoria y el tipo de división realizada.

No se permite realizar la copia elemento a elemento mediante bucles.
