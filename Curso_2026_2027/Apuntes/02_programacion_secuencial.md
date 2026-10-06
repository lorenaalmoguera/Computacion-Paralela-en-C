# Programación secuencial en C

La versión secuencial permite entender el problema, validar su resultado y medir una referencia antes de repartir el trabajo entre procesos.

## 1. Vectores y posiciones

Un vector de `n` elementos tiene índices de `0` a `n-1`. Si `v` es `int*`, `v[i]` accede al entero de índice `i`.

```text
índice:  0      1      2      3
        +------+------+------+
v  ---> | v[0] | v[1] | v[2] | ...
        +------+------+------+
```

No accedas a `v[n]`: queda justo después del vector y no es un elemento válido.

## 2. Argumentos de la línea de comandos

En `int main(int argc, char *argv[])`, `argc` cuenta los argumentos y `argv` contiene sus textos. `argv[0]` suele ser el nombre del programa; el primer argumento del usuario está en `argv[1]`.

Si se esperan un fichero y un tamaño, se necesitan `argc >= 3`: nombre del programa, nombre del fichero y tamaño. Comprueba los argumentos antes de acceder a `argv[1]` y `argv[2]`. `atoi` convierte texto a entero, pero no informa bien de errores de formato; para programas más robustos puede usarse `strtol`.

## 3. Ficheros binarios y `fread`

En texto, los números se representan con caracteres y separadores. El número de bytes depende de sus dígitos y del formato. En binario se guarda la representación de los objetos; cada valor ocupa el tamaño correspondiente a su tipo en ese formato y plataforma.

```c
FILE *f = fopen(nombre, "rb");
```

`"rb"` abre para lectura binaria. Si `fopen` falla devuelve `NULL`; no uses `fread` con ese valor.

```c
fread(destino, tamaño_de_elemento, cantidad, f);
```

El primer argumento es la **dirección de memoria** donde escribir. Si el bucle lee un entero por iteración, el patrón es:

```c
for (int i = 0; i < tam; i++) {
    fread(v + i, sizeof *v, 1, f);
}
```

`v + i` señala el destino del entero `i`. Si se hace una lectura por llamada, `cantidad` es 1. En código robusto se compara el valor devuelto por `fread` con la cantidad esperada.

La posición del fichero avanza tras cada lectura correcta. Si el fichero almacena primero todos los elementos de `V1` y luego los de `V2`, se leen ambos vectores en ese orden. Si solo lees un prefijo de V1, debes saltar o consumir el resto de V1 antes de leer V2: el segundo vector comienza tras el bloque completo del fichero, no tras el prefijo solicitado. Si los datos están intercalados, el orden de lectura debe reflejarlo.

## 4. Cuánto cabe en un fichero

Si el fichero contiene únicamente dos vectores iguales y ocupa 400 bytes, cada vector recibe 200 bytes. Si cada `int` ocupa 4 bytes, caben 50 enteros por vector, 100 enteros en total.

```text
400 bytes
┌───────────────────────┬───────────────────────┐
│ V1: 200 bytes         │ V2: 200 bytes         │
│ 50 int de 4 bytes     │ 50 int de 4 bytes     │
└───────────────────────┴───────────────────────┘
```

Usa `sizeof(int)` en la máquina y comprueba que coincide con el formato del fichero. El enunciado P0_T2 indica hasta 200 enteros, cifra que hay que reconciliar con los 400 bytes y con el tamaño real de cada entero. No leas más elementos de los que contiene el fichero.

## 5. Memoria automática y memoria dinámica

Una declaración como `int v[tam]` crea un array de longitud variable con duración automática: deja de existir al salir de su bloque. Su soporte depende del estándar y del compilador.

`malloc` reserva almacenamiento dinámico que permanece disponible hasta llamar a `free`. Suele decirse que se asigna en el heap, aunque el estándar de C lo describe como almacenamiento asignado dinámicamente.

```c
int *v = malloc((size_t)tam * sizeof *v);
/* usar v[0] ... v[tam - 1] */
free(v);
```

La expresión `sizeof *v` ayuda a mantener el tamaño correcto si cambia el tipo de `v`. Cada bloque reservado se libera una sola vez. Estos fragmentos suponen `tam > 0` y un tamaño que no desborda `size_t`; antes de usar el resultado hay que comprobar `v != NULL`. Incluye `stdlib.h` para malloc/free.

### Dos reservas o una reserva

Dos bloques independientes:

```c
int *v1 = malloc((size_t)tam * sizeof *v1);
int *v2 = malloc((size_t)tam * sizeof *v2);
/* ... */
free(v1);
free(v2);
```

Un bloque contiguo para los dos:

```c
int *datos = malloc((size_t)2 * tam * sizeof *datos);
int *v1 = datos;
int *v2 = datos + tam;
/* ... */
free(datos);
```

```text
datos ──> [ V1[0] ... V1[tam-1] | V2[0] ... V2[tam-1] ]
          ^ v1                  ^ v2 = datos + tam
```

`v1` apunta al inicio y `v2` a una posición interior del mismo bloque; se libera solo `datos`, no cada vista por separado. Comprueba `datos != NULL` antes de calcular `datos + tam`; el tamaño de la reserva conjunta también debe ser representable sin desbordamiento.

## 6. Encontrar máximos y comparar vectores

Para hallar el máximo conjunto, se puede recorrer cada vector y mantener el mayor valor visto, o recorrer todo el bloque contiguo. Inicializar el máximo con `v1[0]` es más general que inventar un límite como `-999999`; requiere que el tamaño sea mayor que cero.

Para contar posiciones en las que `V1` es estrictamente mayor que `V2`, compara parejas con el mismo índice. Los empates no cuentan. Al final el contador está entre 0 y `tam`.

## 7. Índices y aritmética de punteros

Para un puntero `v` a elementos `int`:

```c
v[i]       /* valor de la posición i */
*(v + i)   /* el mismo valor */
&v[i]      /* dirección de la posición i */
v + i      /* la misma dirección */
```

`v[i]` se define como `*(v + i)`. El operador `&` obtiene la dirección del elemento. Por eso `&v[i]` y `v + i` sirven ambos como destino para `fread`. Si el ejercicio pide no usar corchetes, expresa accesos como `*(v+i)`; si pide la variante con índices, usa `v[i]` y, cuando una función necesita la dirección, `&v[i]`.

Sumar uno a un puntero avanza un elemento del tipo apuntado: `int* + 1` avanza `sizeof(int)` bytes; `char* + 1`, un byte. C estándar no permite aritmética sobre `void*`.

## 8. Punteros de tipos distintos y representación

Se puede inspeccionar o modificar la representación en bytes de un objeto mediante `char*` o `unsigned char*`. Si `pcharV1` apunta al comienzo de un `int`, cambiar `*pcharV1` modifica un byte del entero; no garantiza que el entero completo pase a cero. El valor visible depende de la representación y el orden de bytes.

Con `int*`, `*pintV1 = 0` escribe el primer entero completo.

Convertir la dirección de un vector `int` a `double*` no hace que allí exista un objeto `double`. Desreferenciarlo para leer o escribir puede infringir la alineación y las reglas de tipo efectivo; además `double` puede ocupar varios `int`. La práctica, tal como está redactada, pide un acceso que no es portable ni definido por C estándar. Conviene pedir al profesor qué resultado espera como demostración y no presentar esa operación como segura.

## 9. Lista de comprobación secuencial

- ¿He validado `argc` antes de leer `argv`?
- ¿He abierto el fichero y comprobado que `f != NULL`?
- ¿La memoria reservada alcanza para todos los elementos que leeré?
- ¿La disposición real de los datos coincide con el orden de lectura?
- ¿Cada `fread` escribe en una dirección válida y solicita la cantidad correcta?
- ¿Estoy dentro de los índices `0` a `tam-1`?
- ¿He cerrado el fichero y liberado exactamente cada bloque reservado?
- ¿Puedo contrastar el resultado con un ejemplo pequeño calculado a mano?
