# P0_T2 — Memoria dinámica para vectores

## Enunciado

Se dispone de un fichero binario que contiene dos vectores de datos almacenados consecutivamente. El tamaño de ese fichero es de 400 bytes. El programa deberá trabajar con dos vectores, `V1` y `V2`, que tendrán el mismo tamaño.

La cantidad de datos de tipo entero que se deben leer se indicará mediante un argumento en la línea de ejecución del programa. Se sabe que dicha cantidad será como máximo de 200 enteros.

El objetivo será determinar el valor máximo considerando conjuntamente todos los elementos de `V1` y `V2`.

Realiza la tarea anterior de dos formas distintas:

- Utilizando dos reservas de memoria dinámica, una para `V1` y otra para `V2`.
- Utilizando una sola reserva de memoria dinámica para almacenar `V1` y `V2`, del doble de tamaño que en el caso anterior.

## Preguntas y respuestas

### 1. Suponiendo que cada vector contiene 200 elementos, ¿qué representa el acceso `V1[205]` en cada implementación? ¿Es válido en ambos casos?

**Con dos reservas separadas:** `V1` apunta al inicio de una reserva con 200 elementos. El índice 205 queda fuera de esa reserva. Acceder a `V1[205]` tiene comportamiento indefinido; no es una forma válida de llegar a `V2`.

**Con una reserva conjunta:** si los 400 enteros están en un único bloque y `V1` apunta al primer elemento de ese bloque, `V1[205]` señala el elemento número 205 contando desde el comienzo (índice 0-based). Está dentro del bloque conjunto y corresponde a `V2[5]`, suponiendo que `V2` comienza en el índice 200. Aunque la dirección queda dentro de la reserva, acceder a un elemento de `V2` a través de `V1[205]` confunde los límites lógicos de los vectores; lo claro es escribir `V2[5]`.

Por tanto, en el caso separado el acceso está fuera de los límites de la reserva de `V1`. En el caso conjunto cae dentro del bloque total y corresponde a la sexta posición de `V2`, bajo la disposición indicada.

### 2. En la implementación que utiliza una única reserva de memoria, ¿qué relación existe entre las direcciones de `V1` y `V2`?

Si ambos vectores se almacenan consecutivamente en una misma reserva, `V1` apunta al comienzo del bloque y `V2` al elemento que sigue al último de `V1`. Si cada vector tiene `tam` elementos:

```c
V2 = V1 + tam;
```

La dirección de `V2` está desplazada `tam * sizeof(int)` bytes respecto a la dirección de `V1`. Para 200 enteros de 4 bytes, el desplazamiento es de 800 bytes.

## Nota

Este documento responde las preguntas conceptuales; no desarrolla el ejercicio de lectura del fichero ni el cálculo del máximo.
