# Esquema de programación de la asignatura

> Criterios indicados por el profesor para programar y depurar los ejercicios. En estos ejercicios se trabaja sin control de errores y con las funciones y el tipo de comunicación autorizados por el enunciado.

## 1. Sin control de errores

Los ejercicios se escriben **sin control de errores**, siguiendo el criterio de la asignatura. El código debe centrarse en el reparto del trabajo, las reservas, las comunicaciones y el cálculo.

Se parte de los datos y condiciones válidos del ejercicio. Antes de programar hay que anotar esas condiciones: número de procesos, tamaño de los datos, raíz, tipos y distribución solicitada.

## 2. Identificar siempre al proceso que imprime

Cada mensaje debe indicar el rango del proceso que lo escribe:

```c
printf("[P%d] Recibo %d elementos desde P%d, tag=%d\n",
       rank, cantidad, origen, tag);

printf("[P%d] Inicio=%d, cantidad=%d, resultado local=%d\n",
       rank, inicio, cantidad, resultado_local);
```

El rango se interpreta dentro del comunicador utilizado. Si intervienen varios comunicadores, indicar también cuál se está mostrando:

```c
printf("[WORLD P%d | COMM2 P%d] Cantidad local=%d\n",
       rank_world, rank_local, cantidad_local);
```

Las líneas de varios procesos pueden aparecer mezcladas. Su orden en pantalla no demuestra por sí solo el orden en que se ejecutaron las comunicaciones.

## 3. Diseñar primero el reparto paralelo

Antes del código MPI, concretar:

1. Qué calcula el programa secuencial.
2. Cómo se divide el trabajo y qué datos necesita cada proceso.
3. Qué proceso tiene la entrada al principio.
4. Qué memoria necesita cada participante.
5. Qué comunicaciones permiten obtener los datos locales.
6. Cómo se calcula y dónde termina el resultado final.

Preparar una tabla antes de escribir los bucles:

| Proceso | Inicio global | Cantidad de datos | Datos que recibe | Cálculo local | Resultado que entrega |
|---|---:|---:|---|---|---|
| P0 | Según reparto | Según reparto | Su bloque o entrada inicial | Operación del ejercicio | Parcial de P0 |
| P1 | Según reparto | Según reparto | Su bloque | Misma operación o tarea asignada | Parcial de P1 |
| … | … | … | … | … | … |

La raíz también puede calcular. Su participación debe quedar decidida en el diseño; no se supone que solo coordina ni que siempre recibe la misma cantidad que los demás.

## 4. Depurar cada proceso sin presuponer su estado

Seguir por separado la ejecución de **cada proceso**. Para cada uno, comprobar:

- Qué valores tienen sus variables y dónde se inicializan.
- Qué memoria ha reservado y qué índices puede utilizar.
- Cuántos elementos espera y de qué proceso.
- Qué llamada ejecuta antes y después de cada comunicación.
- Qué resultado local obtiene y dónde lo entrega.

No asumir que una variable leída o calculada por P0 existe con ese valor en los demás: cada proceso tiene su memoria. Si otro proceso necesita el valor, debe obtenerlo mediante el mecanismo previsto.

No asumir que todos reciben la misma cantidad, que todos tienen el resto o que un puntero reservado en un proceso está reservado en los demás.

Depurar primero ejemplos pequeños calculables a mano. Incluir casos con reparto exacto y con resto, y estudiar los procesos con cantidad cero si el reparto y el ejercicio los permiten.

## 5. Respetar el tipo de comunicación exigido

| Si el enunciado pide… | El programa debe… |
|---|---|
| Comunicaciones colectivas | Utilizar las colectivas adecuadas que estén permitidas y vistas en clase. |
| Comunicaciones punto a punto | Construir el patrón con los envíos y recepciones permitidos. |

**En un ejercicio exclusivamente punto a punto no se utiliza `MPI_Bcast`**, ni se introduce `Scatter`, `Gather`, `Reduce`, `Barrier` u otra colectiva para simplificar una fase.

Si hay que difundir un dato usando punto a punto, se diseña su distribución mediante envíos y recepciones. Si el ejercicio pide colectivas, se elige la operación correspondiente y se garantiza que participan todos los miembros de su comunicador en el orden necesario.

En las prácticas se utilizan únicamente funciones de MPI-1 vistas en la asignatura. Que una función pertenezca al estándar MPI-1 no implica por sí solo que esté autorizada.

## 6. Reserva de memoria diferencial mediante if / else

La reserva debe reflejar las necesidades de cada rol y expresarse con un `if / else`. No se reserva toda la entrada en todos los procesos por comodidad.

Ejemplo de diseño: P0 dispone de la entrada completa y del vector de parciales; los demás solo necesitan su bloque y su parcial escalar. Este fragmento supone que `N`, `nproces` y `cantidad_local` ya son conocidos donde se utilizan:

```c
int *entrada = NULL;
int *local = NULL;
int *parciales = NULL;
int resultado_local;

if (rank == 0) {
    entrada = malloc((size_t)N * sizeof *entrada);
    local = malloc((size_t)cantidad_local * sizeof *local);
    parciales = malloc((size_t)nproces * sizeof *parciales);
} else {
    local = malloc((size_t)cantidad_local * sizeof *local);
}
```

Aquí P0 utiliza un bloque local independiente para su cálculo. Otro diseño podría trabajar directamente sobre su porción de la entrada, pero debe quedar explícito y liberar la memoria según la propiedad de cada bloque.

Si las cantidades se deciden en la raíz, primero se comunica a cada trabajador su tamaño y después se reserva su bloque. **La información necesaria para reservar tiene que estar disponible antes de la reserva**. Esa comunicación de metadatos puede constituir una fase previa al bloque de reserva de datos.

Para matrices, respetar además la organización exigida por el profesor: cuando pide reservas independientes por filas, no sustituirlas por un bloque contiguo con tabla de filas.

## 7. Diseñar el patrón: qué enviamos y qué recibimos

Describir cada comunicación antes de programarla:

| Fase | Origen | Destino | Contenido | Cantidad y tipo | Etiqueta o colectiva |
|---|---|---|---|---|---|
| Metadatos | Raíz | Trabajador | Cantidad local, dimensiones o inicio si los necesita | Según diseño | Identificador de esa fase |
| Reparto | Raíz | Trabajador | Bloque de entrada | Cantidad asignada y tipo del dato | Identificador de datos |
| Recogida | Trabajador | Raíz | Resultado parcial | Según el resultado | Identificador de resultados |

Para punto a punto, cada envío debe tener su recepción correspondiente: origen/destino, etiqueta y comunicador deben emparejar. Los tipos deben ser compatibles y el búfer de recepción debe tener capacidad suficiente.

```text
P0: preparar datos ── enviar bloque a P1 ── calcular su bloque ── recibir parcial
P1: reservar bloque ── recibir desde P0 ─── calcular su bloque ── enviar parcial
```

El orden debe permitir que las comunicaciones se completen sin depender de que MPI tenga un búfer interno disponible. Revisar si hay ciclos en los que todos esperan que otro publique una recepción.

Para colectivas, anotar qué argumentos corresponden a cada proceso, la raíz si la hay, las cantidades y los desplazamientos. La raíz también participa.

## 8. Saber programar distintos repartos, incluido el resto

Hay que poder calcular el reparto **en código secuencial antes de añadir MPI**. El cálculo produce al menos `cantidad[p]` e `inicio[p]` para cada proceso.

Sea `N` el número de elementos y `P` el de procesos participantes. En los ejemplos por bloques se supone `P > 0`:

```c
int base = N / P;
int resto = N % P;
```

### 8.1. Todo el resto al último proceso

```c
for (int p = 0; p < P; ++p) {
    cantidad[p] = base;
    if (p == P - 1)
        cantidad[p] += resto;
}
```

Con `N=11`, `P=4`: cantidades `[2,2,2,5]`.

### 8.2. Todo el resto al primer proceso

```c
for (int p = 0; p < P; ++p) {
    cantidad[p] = base;
    if (p == 0)
        cantidad[p] += resto;
}
```

Con `N=11`, `P=4`: cantidades `[5,2,2,2]`.

### 8.3. Resto equilibrado entre los primeros procesos

```c
for (int p = 0; p < P; ++p)
    cantidad[p] = base + (p < resto);
```

Con `N=11`, `P=4`: cantidades `[3,3,3,2]`.

### 8.4. Resto equilibrado entre los últimos procesos

```c
for (int p = 0; p < P; ++p)
    cantidad[p] = base + (p >= P - resto);
```

Con `N=11`, `P=4`: cantidades `[2,3,3,3]`.

### 8.5. Un proceso recibe un porcentaje y los demás el resto

Supongamos que P0 recibe el porcentaje entero `porcentaje`, entre 0 y 100, y que `P >= 2`. Se decide explícitamente redondear hacia abajo su cantidad y equilibrar lo que queda entre los demás:

```c
cantidad[0] = (int)(((long long)N * porcentaje) / 100);
int pendientes = N - cantidad[0];
int base_otros = pendientes / (P - 1);
int resto_otros = pendientes % (P - 1);

for (int p = 1; p < P; ++p)
    cantidad[p] = base_otros + ((p - 1) < resto_otros);
```

Con `N=11`, `P=4` y un 40 % para P0: cantidades `[4,3,2,2]`. P0 recibe 4 elementos porque se ha elegido redondear hacia abajo 4,4. Si el enunciado exige otro redondeo o colocar todo el resto final en un proceso concreto, se adapta esa decisión.

### 8.6. Cantidades diferentes o reparto ponderado

Si el ejercicio asigna cantidades concretas, porcentajes a varios procesos o pesos diferentes, se construye la tabla de cantidades con esa regla. Debe especificarse cómo se redondea y dónde terminan los elementos sobrantes.

No basta con multiplicar cada porcentaje y truncarlo por separado: pueden quedar elementos sin asignar. La regla de reparto debe cubrir también esos sobrantes.

### 8.7. Calcular los inicios de cualquier reparto por bloques

Una vez determinadas las cantidades:

```c
int siguiente = 0;
for (int p = 0; p < P; ++p) {
    inicio[p] = siguiente;
    siguiente += cantidad[p];
}
```

Así se obtienen bloques consecutivos sin huecos. Por ejemplo:

```text
N=11, cantidades=[3,3,3,2]
inicios=[0,3,6,9]

P0: índices 0..2
P1: índices 3..5
P2: índices 6..8
P3: índices 9..10
```

No utilizar `inicio = rank * base` para cualquier reparto: deja de describir correctamente muchos repartos con cantidades diferentes.

### 8.8. Reparto cíclico

Si el reparto no es por bloques, las fórmulas cambian. En el reparto cíclico simple, el proceso `p` trabaja sobre:

```c
for (int i = p; i < N; i += P) {
    /* Procesar el elemento global i. */
}
```

Los datos asignados a un proceso no son un tramo global consecutivo. El patrón de copia o comunicación debe ajustarse a ello.

### 8.9. Qué debemos comprobar al estudiar cualquier reparto

Sin añadir control de errores al programa del ejercicio, la traza y el cálculo manual deben demostrar que:

- Cada elemento queda asignado al proceso que corresponde.
- No falta ningún dato ni se duplica trabajo accidentalmente.
- Las cantidades suman `N` cuando el reparto es exclusivo y completo.
- Los índices locales y globales se relacionan correctamente.
- Las reservas, copias y comunicaciones utilizan esas cantidades reales.

La competencia que se exige es saber traducir **cualquier regla de reparto que indique el enunciado** a un cálculo secuencial. Las variantes anteriores sirven como patrones; no sustituyen la lectura del reparto concreto.

## 9. Organizar el código por bloques

```text
1. Inicialización y datos del problema
   └─ MPI_Init, rango, número de procesos y parámetros

2. Diseño y cálculo del reparto
   └─ cantidades, inicios y roles

3. Comunicación previa de metadatos, si hace falta
   └─ qué necesita saber cada proceso para reservar

4. Reserva diferencial de memoria
   └─ if / else según el rol

5. Preparación de la entrada
   └─ lectura o inicialización en el proceso que corresponda

6. Comunicación de los datos
   └─ reparto mediante colectivas o punto a punto, según el enunciado

7. Cálculo secuencial local
   └─ cada proceso calcula con sus propios datos

8. Recolección y combinación de resultados
   └─ recopilar, reducir o combinar según el problema

9. Presentación del resultado
   └─ cada impresión identifica al proceso

10. Liberación de memoria y finalización
    └─ cada proceso libera sus reservas y completa su uso de MPI
```

Los bloques deben verse en el código mediante su orden y comentarios claros. Si se necesita una fase de metadatos antes de reservar, se muestra como tal; no se utiliza memoria cuyo tamaño todavía no se conoce.

El cálculo local tiene que poder entenderse como un algoritmo secuencial independiente de los mensajes que le proporcionaron los datos.

## 10. Controlar los tags en punto a punto

Reservar etiquetas con un significado explícito:

```c
const int TAG_META = 10;
const int TAG_DATOS = 20;
const int TAG_RESULTADO = 30;
```

No es obligatorio dar un tag distinto a cada proceso: origen, destino y comunicador también forman parte del emparejamiento. Sí es necesario que el protocolo evite confundir fases o mensajes.

| Mensaje | Envío | Recepción compatible |
|---|---|---|
| Metadatos | Raíz → trabajador, `TAG_META` | Trabajador recibe de la raíz con `TAG_META`. |
| Bloque de entrada | Raíz → trabajador, `TAG_DATOS` | Trabajador recibe su bloque con `TAG_DATOS`. |
| Parcial | Trabajador → raíz, `TAG_RESULTADO` | Raíz recibe ese parcial con `TAG_RESULTADO`. |

Si la raíz utiliza `MPI_ANY_SOURCE`, consultar `status.MPI_SOURCE` para saber quién entregó el resultado. No suponer que el primero recibido corresponde a P1 ni colocarlo en una posición equivocada.

Si se utiliza `MPI_ANY_TAG`, consultar `status.MPI_TAG` y actuar según el protocolo. Los comodines no eliminan la necesidad de diseñar qué mensajes pueden llegar.

En la depuración imprimir origen o destino, etiqueta y cantidad en los puntos relevantes. Una etiqueta incorrecta puede dejar una recepción esperando un mensaje que nunca llegará.

## Plantilla mental antes de entregar

```text
¿Quién tiene los datos al principio?
          ↓
¿Qué parte procesa cada proceso, incluido el resto?
          ↓
¿Qué memoria reserva cada rol con su if / else?
          ↓
¿Qué envía y qué recibe cada proceso, con qué cantidad y tag?
          ↓
¿Qué cálculo secuencial hace sobre sus datos locales?
          ↓
¿Cómo se recogen y combinan los resultados?
          ↓
¿Puedo seguir la traza de cada proceso sin presuponer su estado?
```

[Todos los apuntes de la asignatura](TODOS_APUNTES_ASIGNATURA.md) · [Memoria de matrices y comunicaciones](../Practica/Secuencial/MEMORIA_MATRICES_RESERVAS_Y_COMUNICACIONES.md).
