# P0_T6 — Programación secuencial

## Enunciado

Haz un programa en C que lea de un archivo binario un total de **80 bytes** (tipo `char` o tipo `uint8_t`) en un vector `VectIncrementando` y copie ese vector a un vector `VectDecrementando`.

El programa debe hacer lo siguiente siempre que se pueda, es decir, siempre que existan todos los índices del vector involucrados en el cálculo:

```text
Si v[i] es mayor que (v[i-1] + v[i-2] - v[i-3]), entonces:
    v[i] = (uint8_t)(v[i-1] + v[i-2] - v[i-3])
De lo contrario:
    v[i] = v[i] / 2
```

Debe implementarse de dos formas:

- Con un bucle iterador que aumente la variable de iteración (normalmente `i`), trabajando sobre `VectIncrementando`.
- Con un bucle iterador que disminuya la variable de iteración, trabajando sobre `VectDecrementando`.

Realiza las tareas necesarias para contestar a las siguientes preguntas.

## Preguntas

1. ¿Qué elementos no se han procesado (porque no se podía) de `VectIncrementando`?
2. ¿Qué elementos no se han procesado (porque no se podía) de `VectDecrementando`?
3. ¿Cuántos elementos coinciden en valor en `VectIncrementando` y `VectDecrementando`?
4. ¿Qué opciones plantearías para dividir el trabajo y poder acelerarlo con HPC?
