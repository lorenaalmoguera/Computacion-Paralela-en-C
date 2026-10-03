# Índices y aritmética de punteros en C

En C se puede acceder a los elementos de un vector usando índices (`v[i]`) o aritmética de punteros (`*(v + i)`). Ambas expresiones acceden al mismo elemento, pero se escriben de forma distinta.

## Ejemplo con un vector

```c
int v[] = {10, 20, 30};
```

- `v[0]` y `*(v + 0)` valen `10`.
- `v[1]` y `*(v + 1)` valen `20`.
- `v[2]` y `*(v + 2)` valen `30`.

La expresión `v + i` es una dirección: empieza en el primer elemento y avanza `i` elementos de tipo `int`. Al desreferenciarla con `*`, se obtiene el valor que hay en esa posición. Por eso `v[i]` equivale a `*(v + i)`.

## Leer con `fread`

`fread` necesita recibir la dirección donde escribirá los datos. Si se lee un entero cada vez dentro de un bucle, la posición de destino debe cambiar en cada iteración.

Con aritmética de punteros:

```c
fread(v + i, sizeof(int), 1, f);
```

Con índices:

```c
fread(&v[i], sizeof(int), 1, f);
```

`v[i]` representa el valor del elemento, mientras que `&v[i]` representa su dirección. Se necesita `&v[i]` porque `fread` escribe en memoria. También se podría usar `&(*(v + i))`, pero se simplifica a `v + i`.

## Dos vectores

Cada vector tiene su propia dirección inicial. Para leer la posición `i` en ambos, se usa la dirección de esa posición en cada vector:

```c
fread(&v1[i], sizeof(int), 1, f);
fread(&v2[i], sizeof(int), 1, f);
```

En la primera iteración (`i == 0`) se escribe en `v1[0]` y `v2[0]`; en la siguiente (`i == 1`), en `v1[1]` y `v2[1]`. `fread` avanza por el fichero al leer, mientras que `i` selecciona la posición de memoria de destino.

La versión con índices permite ver explícitamente la posición mediante `[i]`. La versión con aritmética de punteros expresa esa posición avanzando desde la dirección inicial. En este ejemplo, ambas hacen lo mismo.
