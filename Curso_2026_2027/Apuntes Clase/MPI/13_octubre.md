Seguimos con las funciones colectivas

## MPI_Reduce_scatter

`MPI_Reduce_scatter(send, receive, recv_cnt, MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD)`


**Reduce y reparte:** cada proceso aporta un vector; MPI combina los valores posición a posición y distribuye el resultado en bloques consecutivos entre los procesos.

```c
int MPI_Reduce_scatter(const void *sendbuf, void *recvbuf,
                       const int recvcounts[], MPI_Datatype datatype,
                       MPI_Op op, MPI_Comm comm);

// Ejemplo de llamada en C:
MPI_Reduce_scatter(send, receive, recv_cnt,
                   MPI_INT, MPI_SUM, MPI_COMM_WORLD);
```

> En C, para elementos `int` usamos `MPI_INT`. El `MPI_INTEGER` del apunte inicial corresponde al tipo `INTEGER` de Fortran.

### Qué es cada argumento

| Argumento | Significado | Ejemplo con 3 procesos |
|---|---|---|
| `send` (`sendbuf`) | Vector de entrada local de cada proceso. Contiene tantos elementos como la suma de `recv_cnt`. | Cada proceso aporta 4 enteros. |
| `receive` (`recvbuf`) | Buffer local donde se escribe el bloque que recibe ese proceso. Debe tener capacidad para `recv_cnt[rank]` elementos. | P0 recibe 1; P1, 2; P2, 1. |
| `recv_cnt` (`recvcounts`) | Array con una entrada por proceso: la entrada `i` indica cuántos elementos recibe el rango `i`. Debe ser idéntico en todos. | `{1, 2, 1}`; cuenta elementos, no bytes. |
| `MPI_INT` (`datatype`) | Tipo MPI de los elementos. | Enteros `int` en C. |
| `MPI_SUM` (`op`) | Operación que combina valores de la misma posición. | Suma. |
| `MPI_COMM_WORLD` (`comm`) | Comunicador que determina el grupo participante. | Los 3 procesos del programa. |

Es **colectiva**: todos los procesos del comunicador deben llamarla en el mismo orden de llamadas colectivas. **No hay `root`**. La función devuelve un código de estado (`MPI_SUCCESS` si tiene éxito); el bloque recibido se guarda en `receive`.

### Ejemplo paso a paso

Tenemos 3 procesos y `recv_cnt = {1, 2, 1}`. Cada uno aporta **1 + 2 + 1 = 4 elementos**, aunque luego reciba menos.

| Entrada | Posición 0 | Posición 1 | Posición 2 | Posición 3 |
|---|---:|---:|---:|---:|
| P0 | 1 | 2 | 3 | 4 |
| P1 | 10 | 20 | 30 | 40 |
| P2 | 100 | 200 | 300 | 400 |
| **Resultado de sumar por columna** | **111** | **222** | **333** | **444** |

Por ejemplo, la posición 0 se calcula como `1 + 10 + 100 = 111`. Se suman las columnas entre procesos, no los elementos de cada fila.

```mermaid
flowchart TB
    subgraph entrada["1 · Vectores locales de entrada"]
        direction LR
        A["P0 · send<br/>[1, 2, 3, 4]"]
        B["P1 · send<br/>[10, 20, 30, 40]"]
        C["P2 · send<br/>[100, 200, 300, 400]"]
    end
    A --> R
    B --> R
    C --> R
    R["2 · REDUCE · MPI_SUM<br/>Sumar posición a posición<br/>[111, 222, 333, 444]"]
    R --> S["3 · SCATTER · recv_cnt = [1, 2, 1]<br/>Separar en bloques consecutivos<br/>[111] │ [222, 333] │ [444]"]
    subgraph salida["4 · Resultado local en receive"]
        direction LR
        D["P0<br/>[111]"]
        E["P1<br/>[222, 333]"]
        F["P2<br/>[444]"]
    end
    S -->|"1 elemento"| D
    S -->|"2 elementos"| E
    S -->|"1 elemento"| F
    classDef input fill:#dbeafe,stroke:#2563eb,color:#172554,stroke-width:2px;
    classDef reduce fill:#ede9fe,stroke:#7c3aed,color:#2e1065,stroke-width:2px;
    classDef scatter fill:#ffedd5,stroke:#ea580c,color:#431407,stroke-width:2px;
    classDef output fill:#dcfce7,stroke:#16a34a,color:#052e16,stroke-width:2px;
    class A,B,C input;
    class R reduce;
    class S scatter;
    class D,E,F output;
```

El diagrama representa el resultado lógico; MPI puede usar distintos algoritmos internos y no necesita reunir primero el vector en un proceso central.

El reparto sigue el orden de los rangos: P0 recibe la posición 0, P1 las posiciones 1 y 2, y P2 la posición 3. El bloque del rango `i` empieza en la suma de los tamaños de los bloques anteriores. En su buffer local, cada bloque se guarda desde `receive[0]`.

### Programa completo en C

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0)
            fprintf(stderr, "Ejecuta con 3 procesos.\n");
        MPI_Finalize();
        return 1;
    }

    int recv_cnt[3] = {1, 2, 1};
    int factor = (rank == 0) ? 1 : (rank == 1) ? 10 : 100;
    int send[4];
    for (int j = 0; j < 4; ++j)
        send[j] = factor * (j + 1);

    // Capacidad para el mayor bloque: 2 enteros.
    // Cada proceso utiliza solo recv_cnt[rank] posiciones.
    int receive[2];
    MPI_Reduce_scatter(send, receive, recv_cnt,
                       MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("P%d recibe:", rank);
    for (int j = 0; j < recv_cnt[rank]; ++j)
        printf(" %d", receive[j]);
    printf("\n");

    MPI_Finalize();
    return 0;
}
```

Si guardas el código en `reduce_scatter.c`, en un entorno con MPI:

```bash
mpicc reduce_scatter.c -o reduce_scatter
mpirun -np 3 ./reduce_scatter
```

Resultado esperado (las líneas pueden aparecer en otro orden o intercalarse):

```text
P0 recibe: 111
P1 recibe: 222 333
P2 recibe: 444
```

**Para recordarlo:** todos aportan un vector completo de contribuciones, se reduce por posiciones y cada uno recibe un trozo. `MPI_Reduce` entrega el resultado completo a un `root`; `MPI_Allreduce`, completo a todos; `MPI_Reduce_scatter`, repartido entre todos.

Referencia: [documentación oficial de Open MPI](https://docs.open-mpi.org/en/v5.0.10/man-openmpi/man3/MPI_Reduce_scatter.3.html).


## MPI_Allgather

`MPI_Allgather` recopila una cantidad **igual** de datos de cada proceso y deja la concatenación ordenada por rango en el buffer de **todos** los procesos. No hay `root`: todos aportan y todos reciben.

```c
int MPI_Allgather(const void *sendbuf, int sendcount,
                  MPI_Datatype sendtype, void *recvbuf, int recvcount,
                  MPI_Datatype recvtype, MPI_Comm comm);
```

| Argumento | Significado |
|---|---|
| `sendbuf`, `sendcount`, `sendtype` | Buffer local, cantidad enviada por este proceso y tipo de esos elementos. Todos envían la misma cantidad y tipo compatible. |
| `recvbuf` | Buffer local con espacio para un bloque recibido de cada proceso. |
| `recvcount`, `recvtype` | Cantidad y tipo que se reciben **de cada proceso**; deben concordar con lo que envía cada origen. |
| `comm` | Comunicador del grupo participante. |

Con `p` procesos, cada `recvbuf` necesita `p * recvcount` elementos. El bloque del rango `i` queda en la posición `i * recvcount` (en unidades del tipo recibido). Todos deben llamar a la colectiva en el mismo orden.

Ejemplo

```Text

P0 -> 10 , 11
P1 -> 20 , 21
P2 -> 30 , 31
P3 -> 40 , 41

primero hacemos la recoleción según rango

10 , 11 , 20 , 21 , 30 , 31 , 40 , 41

PERO all ser un MPI_Allgather todos recibirán el resultado, por lo tanto

P0 -> 10 , 11 , 20 , 21 , 30 , 31 , 40 , 41
P1 -> 10 , 11 , 20 , 21 , 30 , 31 , 40 , 41
P2 -> 10 , 11 , 20 , 21 , 30 , 31 , 40 , 41
P3 -> 10 , 11 , 20 , 21 , 30 , 31 , 40 , 41

```

### Diagrama de `MPI_Allgather`

Cada proceso aporta el mismo número de elementos (en este ejemplo, dos). MPI los coloca en el resultado siguiendo el rango de los procesos y entrega el vector completo a **todos**.

```mermaid
flowchart TB
    subgraph envios["1 · Cada proceso envía 2 valores"]
        direction LR
        A["P0<br/>[10, 11]"]
        B["P1<br/>[20, 21]"]
        C["P2<br/>[30, 31]"]
        D["P3<br/>[40, 41]"]
    end
    A --> G
    B --> G
    C --> G
    D --> G
    G["2 · ALLGATHER<br/>Concatenar por rango<br/>[10, 11 | 20, 21 | 30, 31 | 40, 41]"]
    G --> O
    subgraph copias["3 · Todos reciben el vector completo"]
        direction LR
        O["P0, P1, P2 y P3<br/>[10, 11, 20, 21, 30, 31, 40, 41]"]
    end
    classDef input fill:#dbeafe,stroke:#2563eb,color:#172554,stroke-width:2px;
    classDef gather fill:#ede9fe,stroke:#7c3aed,color:#2e1065,stroke-width:2px;
    classDef output fill:#dcfce7,stroke:#16a34a,color:#052e16,stroke-width:2px;
    class A,B,C,D input;
    class G gather;
    class O output;
```

## MPI_Allgatherv

`MPI_Allgatherv` hace lo mismo que `MPI_Allgather`, pero cada proceso puede aportar una cantidad distinta de elementos. Cada proceso receptor proporciona sus propios arrays para describir cómo ordenar los bloques en su buffer. No hay `root` del que dependan los desplazamientos.

```c
int MPI_Allgatherv(const void *sendbuf, int sendcount,
                   MPI_Datatype sendtype, void *recvbuf,
                   const int recvcounts[], const int displs[],
                   MPI_Datatype recvtype, MPI_Comm comm);
```

| Argumento | Significado |
|---|---|
| `sendbuf`, `sendcount`, `sendtype` | Datos que aporta este proceso, número de elementos y tipo. `sendcount` puede variar por proceso. |
| `recvbuf` | Buffer local que reúne todos los bloques. Debe tener capacidad suficiente para cubrir los desplazamientos y tamaños. |
| `recvcounts[i]` | Cuántos elementos recibir del rango `i`. En cada receptor, debe concordar con lo que ese rango envía. |
| `displs[i]` | Posición inicial del bloque de rango `i` en `recvbuf`, medida en unidades de `recvtype`. |
| `recvtype`, `comm` | Tipo de los datos recibidos y comunicador participante. |

Todos envían y todos reciben la colección completa, pero cada proceso decide localmente sus `recvcounts` y `displs`. En una disposición compacta, `displs[i]` suele ser la suma de los conteos anteriores. Los huecos son válidos si hay espacio; las regiones escritas no deben solaparse.

Para calcular conteos y desplazamientos, cada proceso puede construir arrays como estos:

```c
for(int i = 0 ; i < 3 ; i++){
    recv_cnt[i] = 2 + i;
    desp_cnt[i] = 4*i + 1; // posiciones iniciales en el buffer receptor
}
```
Los conteos pueden ser distintos entre rangos, por eso se pasa un array `recvcounts`, incluso cuando casualmente todos los bloques tengan tamaño 2. En ese caso los conteos podrían ser `{2, 2, 2, ...}`. La llamada lleva el conteo local enviado (`sendcount`) y cada receptor describe los bloques entrantes con su array.

### Diagrama de `MPI_Allgatherv`

Aquí cada proceso puede enviar una cantidad distinta. En el ejemplo, los tamaños recibidos por rango son `recvcounts = [1, 2, 3, 1]` y los desplazamientos en el buffer son `displs = [0, 1, 3, 6]`. MPI coloca cada bloque en su posición y todos los procesos obtienen el mismo vector final.

```mermaid
flowchart TB
    subgraph enviosv["1 · Cada proceso envía una cantidad variable"]
        direction LR
        A["P0 · 1 valor<br/>[10]"]
        B["P1 · 2 valores<br/>[20, 21]"]
        C["P2 · 3 valores<br/>[30, 31, 32]"]
        D["P3 · 1 valor<br/>[40]"]
    end
    A --> G
    B --> G
    C --> G
    D --> G
    G["2 · ALLGATHERV<br/>recvcounts = [1, 2, 3, 1]<br/>displs = [0, 1, 3, 6]<br/>Colocar cada bloque según su desplazamiento"]
    G --> O
    subgraph resultados["3 · Cada proceso recibe el mismo buffer"]
        direction LR
        O["P0, P1, P2 y P3<br/>[10 | 20, 21 | 30, 31, 32 | 40]"]
    end
    classDef input fill:#dbeafe,stroke:#2563eb,color:#172554,stroke-width:2px;
    classDef gather fill:#ede9fe,stroke:#7c3aed,color:#2e1065,stroke-width:2px;
    classDef output fill:#dcfce7,stroke:#16a34a,color:#052e16,stroke-width:2px;
    class A,B,C,D input;
    class G gather;
    class O output;
```

Los arrays `recvcounts` y `displs` describen, para cada rango, cuántos elementos colocar y desde qué posición del buffer receptor. En este ejemplo, el bloque de P2 comienza en la posición 3 y ocupa las posiciones 3, 4 y 5. Los desplazamientos se cuentan en unidades del tipo de recepción (`MPI_Datatype`); los huecos son posibles si los desplazamientos elegidos los dejan.

## `MPI_Alltoall`

`MPI_Alltoall` realiza un intercambio personalizado entre todos los procesos. Cada proceso divide su vector de envío en bloques iguales, uno para cada destino. El bloque número `j` se envía al proceso de rango `j`. A su vez, cada proceso recibe un bloque de cada origen y los guarda en orden de rango de origen. No reduce ni suma los datos.

```c
int MPI_Alltoall(const void *sendbuf, int sendcount,
                 MPI_Datatype sendtype, void *recvbuf, int recvcount,
                 MPI_Datatype recvtype, MPI_Comm comm);
```

| Argumento | Significado |
|---|---|
| `sendbuf` | Buffer local dividido en bloques, uno para cada destino. |
| `sendcount` | Elementos que este proceso envía a **cada** destino. Es el mismo tamaño para todos los destinos y procesos de esta llamada. |
| `sendtype` | Tipo MPI de cada elemento enviado. |
| `recvbuf` | Buffer que recibe un bloque de cada origen. |
| `recvcount` | Elementos que se reciben de **cada** origen; debe coincidir con lo que cada origen envía a este proceso. |
| `recvtype` | Tipo MPI de cada elemento recibido. |
| `comm` | Comunicador cuyos procesos participan en el intercambio. |

Todos los procesos llaman a la operación. Si hay `p` procesos y `sendcount = k`, cada proceso necesita espacio para `p × k` elementos en ambos buffers. Puede enviar un bloque a sí mismo también. La recepción queda ordenada por rango de origen: el bloque del origen `i` empieza en `recvbuf + i × recvcount` (teniendo en cuenta el tipo de dato).

### Ejemplo con 4 procesos

Cada proceso envía dos enteros a cada destino. Por ejemplo, P0 envía `[10, 11]` a P0, `[12, 13]` a P1, `[14, 15]` a P2 y `[16, 17]` a P3. P1 recibe el segundo bloque de cada origen, P2 el tercero, etc.

```mermaid
flowchart LR
    subgraph envios["Bloques de salida (dos valores a cada destino)"]
        A["P0<br/>P0:[10,11] · P1:[12,13]<br/>P2:[14,15] · P3:[16,17]"]
        B["P1<br/>P0:[20,21] · P1:[22,23]<br/>P2:[24,25] · P3:[26,27]"]
        C["P2<br/>P0:[30,31] · P1:[32,33]<br/>P2:[34,35] · P3:[36,37]"]
        D["P3<br/>P0:[40,41] · P1:[42,43]<br/>P2:[44,45] · P3:[46,47]"]
    end
    A --> X
    B --> X
    C --> X
    D --> X
    A --> Y
    B --> Y
    C --> Y
    D --> Y
    A --> Z
    B --> Z
    C --> Z
    D --> Z
    A --> W
    B --> W
    C --> W
    D --> W
    subgraph recepciones["Cada proceso recibe un bloque de cada origen, en orden"]
        direction TB
        X["P0 recibe<br/>[10,11 | 20,21 | 30,31 | 40,41]"]
        Y["P1 recibe<br/>[12,13 | 22,23 | 32,33 | 42,43]"]
        Z["P2 recibe<br/>[14,15 | 24,25 | 34,35 | 44,45]"]
        W["P3 recibe<br/>[16,17 | 26,27 | 36,37 | 46,47]"]
    end
    classDef process fill:#dbeafe,stroke:#2563eb,color:#172554,stroke-width:2px;
    classDef result fill:#dcfce7,stroke:#16a34a,color:#052e16,stroke-width:2px;
    class A,B,C,D process;
    class X,Y,Z,W result;
```

Se puede leer como una matriz: las **filas** son los procesos que envían y las **columnas** los destinos. Cada proceso receptor recoge su columna, concatenada por rango de origen. En este ejemplo, `sendcount = recvcount = 2`.

## `MPI_Alltoallv`

La variante `v` permite que los bloques enviados y recibidos tengan tamaños distintos. En cada proceso, `sendcounts[j]` indica cuántos elementos enviar al destino `j`, y `sdispls[j]` dónde empieza ese bloque en `sendbuf`. Análogamente, `recvcounts[i]` indica cuántos elementos recibir del origen `i`, y `rdispls[i]` dónde colocarlos en `recvbuf`.

```c
int MPI_Alltoallv(const void *sendbuf, const int sendcounts[],
                  const int sdispls[], MPI_Datatype sendtype,
                  void *recvbuf, const int recvcounts[],
                  const int rdispls[], MPI_Datatype recvtype,
                  MPI_Comm comm);
```

| Argumento | Significado |
|---|---|
| `sendbuf` / `recvbuf` | Buffers locales de envío y recepción. |
| `sendcounts[j]` | Número de elementos que este proceso manda al rango `j`. |
| `sdispls[j]` | Posición inicial del bloque destinado a `j` dentro de `sendbuf`. |
| `sendtype` / `recvtype` | Tipos MPI de los elementos enviados y recibidos. |
| `recvcounts[i]` | Número de elementos que este proceso espera del origen `i`. Debe coincidir con el `sendcounts` de ese origen hacia este rango. |
| `rdispls[i]` | Posición inicial del bloque del origen `i` en `recvbuf`. |
| `comm` | Comunicador cuyos procesos participan. |

Los arrays de conteos y desplazamientos tienen una entrada por proceso, y cada proceso puede tener valores distintos en sus propios arrays. Los desplazamientos se miden en unidades del tipo MPI correspondiente, no en bytes. Todos los procesos participan; tampoco hay reducción ni `root`.

### Ejemplo con cantidades variables

Usamos tres procesos. Cada celda indica lo que el proceso de la fila envía al destino de la columna:

| Origen ↓ / Destino → | P0 | P1 | P2 |
|---|---|---|---|
| **P0** | `[a00]` | `[a01, a02]` | `[]` |
| **P1** | `[b10, b11]` | `[b12]` | `[b13, b14, b15]` |
| **P2** | `[c20]` | `[]` | `[c21, c22]` |

Como ejemplo concreto, en P0: `sendcounts = [1, 2, 0]` y, usando bloques compactos, `sdispls = [0, 1, 3]`. Sus datos de envío son `[a00, a01, a02]`; el desplazamiento del bloque vacío puede señalar el final del buffer. Para recibir, P0 espera `[1, 2, 1]` elementos de P0, P1 y P2, respectivamente.

```mermaid
flowchart LR
    subgraph origenes["Origen: bloques enviados a cada destino"]
        A["P0<br/>a00 → P0<br/>a01,a02 → P1<br/>∅ → P2"]
        B["P1<br/>b10,b11 → P0<br/>b12 → P1<br/>b13,b14,b15 → P2"]
        C["P2<br/>c20 → P0<br/>∅ → P1<br/>c21,c22 → P2"]
    end
    A -->|"1"| X
    A -->|"2"| Y
    B -->|"2"| X
    B -->|"1"| Y
    B -->|"3"| Z
    C -->|"1"| X
    C -->|"2"| Z
    subgraph destinos["Destino: bloques colocados por rango de origen"]
        direction TB
        X["P0 recibe<br/>[a00 | b10,b11 | c20]"]
        Y["P1 recibe<br/>[a01,a02 | b12 | vacío]"]
        Z["P2 recibe<br/>[vacío | b13,b14,b15 | c21,c22]"]
    end
    classDef process fill:#dbeafe,stroke:#2563eb,color:#172554,stroke-width:2px;
    classDef result fill:#dcfce7,stroke:#16a34a,color:#052e16,stroke-width:2px;
    class A,B,C process;
    class X,Y,Z result;
```

Los números sobre las flechas son las cantidades enviadas. La flecha P0→P2 y P2→P1 lleva cero elementos, así que no se dibuja. Cada receptor guarda los bloques entrantes en orden de rango de origen, en las posiciones indicadas por sus `rdispls`. El número de elementos de cada mensaje debe concordar entre el `sendcounts` del emisor y el `recvcounts` del receptor.

### Inciso: ¿cómo cambiar la posición de los ceros en `recvbuf`?

`MPI_Alltoallv` permite decidir **dónde empieza cada bloque recibido** mediante `rdispls`. Si el bloque completo cabe en una zona continua, se puede colocar directamente en la posición deseada. Pero la función no reordena los elementos dentro de un bloque ni deja huecos entre sus elementos: el bloque de cada origen se copia de forma contigua.

Por ejemplo, supongamos que el proceso recibe dos valores de cada uno de los cuatro orígenes:

```text
P0 aporta [10, 11]   P1 aporta [20, 21]
P2 aporta [30, 31]   P3 aporta [40, 41]
```

Queremos construir este buffer:

```text
[10, 11, 0, 0, 0, 21, 0, 0, 30, 0, 0, 0, 40, 41]
```

De P1 queremos conservar solo `21`, y de P2 solo `30`. Como cada bloque recibido tiene dos valores contiguos, primero recibimos los cuatro bloques juntos en un buffer temporal (usando desplazamientos `0, 2, 4, 6`) y después copiamos los valores a las posiciones elegidas. Inicializamos el destino a cero para que las posiciones sin copiar sigan siendo cero:

```c
int recvcounts[4] = {2, 2, 2, 2};
int rdispls[4]    = {0, 2, 4, 6};
int temporal[8];
int resultado[14] = {0};

MPI_Alltoallv(sendbuf, sendcounts, sdispls, MPI_INT,
              temporal, recvcounts, rdispls, MPI_INT,
              MPI_COMM_WORLD);

// temporal está ordenado por origen:
// [10, 11 | 20, 21 | 30, 31 | 40, 41]
resultado[0]  = temporal[0]; // 10, de P0
resultado[1]  = temporal[1]; // 11, de P0
resultado[5]  = temporal[3]; // 21, segundo valor de P1
resultado[8]  = temporal[4]; // 30, primer valor de P2
resultado[12] = temporal[6]; // 40, de P3
resultado[13] = temporal[7]; // 41, de P3
```

Así queda `resultado = [10, 11, 0, 0, 0, 21, 0, 0, 30, 0, 0, 0, 40, 41]`. Los `recvcounts` deben concordar con los datos que los emisores envían a este proceso; si se quiere recibir solo algunos valores directamente, los emisores también deben configurar sus bloques de envío para mandar exactamente esos valores. El buffer temporal es útil cuando se reciben bloques completos y luego se eligen o recolocan elementos.

### ¿Qué pasa si cambiamos `rdispls` de `{0, 4, 8, 12}` a `{0, 4, 2, 12}`?

Supongamos cuatro orígenes, `recvcounts = {4, 4, 4, 4}` y un `recvbuf` de 16 enteros. Cada desplazamiento marca dónde empieza el bloque de 4 elementos del origen correspondiente. Con `{0, 4, 8, 12}`, los bloques quedan seguidos sin solaparse. Al cambiar el desplazamiento de P2 de `8` a `2`, su bloque ocupa las posiciones 2–5 y se solapa con P0 (posiciones 0–3) y P1 (4–7).

```mermaid
flowchart TB
    subgraph correcto["rdispls = {0, 4, 8, 12} · bloques contiguos"]
        direction LR
        A["P0 · índices 0–3"]
        B["P1 · índices 4–7"]
        C["P2 · índices 8–11"]
        D["P3 · índices 12–15"]
        A --- B --- C --- D
    end
    subgraph solapado["rdispls = {0, 4, 2, 12} · P2 se solapa"]
        direction LR
        E["P0 · 0–3"]
        F["P1 · 4–7"]
        G["P2 · 2–5  ⚠️"]
        H["P3 · 12–15"]
        E --- F --- H
        G -. "solapa en 2–3" .-> E
        G -. "solapa en 4–5" .-> F
    end
    classDef safe fill:#dcfce7,stroke:#16a34a,color:#052e16,stroke-width:2px;
    classDef overlap fill:#fee2e2,stroke:#dc2626,color:#450a0a,stroke-width:3px;
    class A,B,C,D safe;
    class E,F,H safe;
    class G overlap;
```

Los bloques de recepción no deben solaparse: dos mensajes intentarían escribir en las mismas posiciones del buffer, lo que hace inválida la operación. Los `rdispls` pueden dejar huecos (por ejemplo, volver a usar `{0, 4, 8, 12}` en un buffer mayor), pero cada región ocupada debe ser distinta. Si se busca elegir valores individuales o ponerlos en posiciones no contiguas, recíbelos primero en un buffer temporal y cópialos después, como en el ejemplo anterior.

### Diferencia rápida

| Operación | ¿Cuántos envía cada proceso a cada destino? | ¿Cómo se eligen los bloques? |
|---|---|---|
| `MPI_Alltoall` | El mismo número fijo. | Bloques iguales, colocados contiguamente por rango. |
| `MPI_Alltoallv` | Puede variar según destino y origen. | Arrays `sendcounts`, `sdispls`, `recvcounts` y `rdispls`. |

# Conceptos teóricos

Puede salir en traza. La recepción puede ser bloqueante.
## MPI_Ssend
Envío síncrono, el envío finaliza cuando empieza la recepción. 
El envío finaliza cuando el receptor COMIENZA a recibir, no cuando lo recibe todo.

## MPI_Bsend
Envío basado en buffer, acaba independientemente del receptor cuando la información está en el buffer.
Los problemas los podemos tener en el buffer.

## MPI_Rsend
Acaba independiente de lo que pasa en recepción, es la función que más errores puede provocar y por tanto optimizar. Tenemos que estar seguros de que el receptor está preparado para recibir.

## MPI_Recv
Cuando se llama a esta función no se retorna hasta que se ha recibido el mensaje (si no llega nunca no retorna nunca)

## MPI_Isend

Una opción para intentar simultanear cómputo y comunicaciones son las funciones asíncronas (se intenta evitar que los procesos estén ociosos mientras se espera un proceso de envío o de recepción)

## MPI_Irecv