# P0_T5 — Máximo por partes con reservas dinámicas

## Enunciado

Se dispone de un fichero binario que contiene datos almacenados consecutivamente. El tamaño de ese fichero es de **4000 bytes**. El programa deberá trabajar con un solo vector de **1000 enteros**, `V1`.

El número de partes en las que se debe dividir (lógicamente) el vector se indicará mediante un argumento en la línea de ejecución del programa. Dicho número será como máximo **10**.

La suma de los tamaños del número de partes indicado coincide exactamente con los **1000 elementos**.

A diferencia del ejercicio anterior, las partes deberán almacenarse de la siguiente forma:

- La primera parte, la que empieza en el elemento 0, permanecerá en el vector original `V1` y se trabajará directamente sobre ella.
- Para cada una de las partes restantes se realizará una nueva reserva de memoria dinámica, con el tamaño exacto necesario para almacenar dicha parte.
- Los elementos correspondientes de `V1` deberán copiarse en cada una de las nuevas zonas de memoria reservadas.
- Una vez realizadas las copias, el cálculo del máximo de cada parte deberá realizarse utilizando la zona de memoria correspondiente a dicha parte y no accediendo de nuevo a sus elementos dentro de `V1`.

Para cada parte se deberá mostrar:

- El índice de comienzo dentro de `V1` (la primera parte comienza en la posición 0).
- El número de elementos que contiene.
- El valor máximo de dicha parte.

Finalmente, utilizando los máximos obtenidos para cada parte, se calculará el máximo global del vector, es decir, un único valor correspondiente al mayor de los 1000 enteros almacenados en `V1`.
