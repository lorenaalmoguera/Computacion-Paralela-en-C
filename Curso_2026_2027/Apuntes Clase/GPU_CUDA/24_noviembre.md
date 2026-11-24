# GPU: unidad de procesamiento gráfico

Una **GPU** (*Graphics Processing Unit*) es un procesador diseñado originalmente para acelerar gráficos. En una escena gráfica se realizan operaciones sobre vértices, polígonos, texturas y píxeles. Muchas de estas operaciones pueden ejecutarse sobre numerosos elementos a la vez, aunque no todos los algoritmos gráficos son independientes: por ejemplo, algunos filtros necesitan consultar píxeles vecinos.

Una imagen puede representarse como una matriz de píxeles. Si queremos aumentar el brillo de cada píxel de manera independiente, se puede repartir el trabajo:

```text
Imagen de entrada               Trabajo paralelo              Imagen de salida
[p0 p1 p2 p3 ...] ──► [procesar p0 | procesar p1 | ...] ──► [q0 q1 q2 q3 ...]
```

Esta abundancia de trabajo similar hizo que las GPU resultaran interesantes también para cálculos científicos, simulaciones e inteligencia artificial. No todos los cálculos por píxel son sencillos: el coste depende del algoritmo.

## CPU y GPU: distintas prioridades

La CPU suele dedicar más recursos por núcleo a ejecutar con rapidez tareas variadas, con decisiones y dependencias. La GPU dedica una gran parte de sus recursos a ejecutar muchas operaciones concurrentes y a ocultar esperas de memoria con otros trabajos disponibles.

Esto **no significa que una GPU tenga menor capacidad de cómputo total**: puede superar ampliamente a una CPU en operaciones paralelas adecuadas. Por el contrario, una tarea pequeña, secuencial o con muchas dependencias puede funcionar mejor en la CPU. El rendimiento depende del problema, la transferencia de datos y el hardware. La eficiencia energética también depende de la aplicación y de la plataforma.

# GPGPU y CUDA

**GPGPU** significa *General-Purpose Computing on Graphics Processing Units*: utilizar una GPU para cálculos de propósito general, no exclusivamente para gráficos. Antes de disponer de herramientas específicas se empleaban APIs gráficas, como OpenGL o DirectX, para adaptar cálculos generales al procesamiento gráfico.

NVIDIA presentó **CUDA** (*Compute Unified Device Architecture*) en 2006 para facilitar la programación de sus GPU. CUDA proporciona un modelo de programación y herramientas para escribir código que se ejecuta en la GPU. Es una plataforma de NVIDIA; no todas las GPU utilizan CUDA.

## Programación CPU + GPU

En el modelo habitual, la CPU actúa como **host**: prepara los datos y lanza las tareas. La GPU actúa como **device** y ejecuta funciones paralelas denominadas *kernels*. Después, la CPU puede utilizar los resultados.

```text
          HOST: CPU y memoria del sistema
          ┌─────────────────────────────┐
          │ 1. Preparar datos           │
          └──────────────┬──────────────┘
                         │ transferir datos cuando sea necesario
                         ▼
          DEVICE: GPU y memoria accesible por la GPU
          ┌─────────────────────────────┐
          │ 2. Ejecutar kernel          │
          └──────────────┬──────────────┘
                         │ devolver resultados cuando sea necesario
                         ▼
          ┌─────────────────────────────┐
          │ 3. CPU utiliza resultados  │
          └─────────────────────────────┘
```

En una GPU discreta, la memoria de la CPU y la memoria propia de la GPU suelen ser físicamente distintas y la transferencia puede tener un coste considerable. En sistemas integrados pueden compartir memoria física; además, CUDA ofrece **memoria unificada**, que simplifica el acceso desde CPU y GPU. Esto no elimina necesariamente los costes de movimiento de datos ni las necesidades de sincronización.

La comparación con MPI ayuda a recordar que los datos deben estar disponibles donde se procesan, pero CUDA y MPI son modelos diferentes: la gestión de memoria y las operaciones de comunicación no son las mismas.

# Arquitectura de una GPU NVIDIA

Una GPU NVIDIA se organiza en varios **SM** (*Streaming Multiprocessors*). Cada SM dispone de unidades de ejecución, como CUDA cores, y de recursos como registros y memoria compartida. Según la arquitectura también incluye otras unidades especializadas, por ejemplo unidades para determinadas funciones matemáticas o cálculos matriciales. El número y la organización exactos varían entre modelos.

```text
GPU NVIDIA
┌─────────────────────────────────────────────────────┐
│  SM 0                     SM 1                 ...  │
│  ┌──────────────────┐    ┌──────────────────┐       │
│  │ unidades de      │    │ unidades de      │       │
│  │ ejecución        │    │ ejecución        │       │
│  │ registros        │    │ registros        │       │
│  │ memoria compartida│   │ memoria compartida│      │
│  └──────────────────┘    └──────────────────┘       │
└─────────────────────┬───────────────────────────────┘
                      │
             memoria global de la GPU
```

En CUDA, el programa organiza los hilos en **bloques** y los bloques en una **rejilla** (*grid*). Un bloque se asigna a un SM para su ejecución; un SM puede alojar varios bloques si sus recursos lo permiten. Los hilos de un mismo bloque pueden cooperar mediante memoria compartida y sincronización apropiada. La memoria compartida de un bloque no es un espacio común para todos los bloques de la GPU.

Los CUDA cores no equivalen directamente a núcleos de CPU: tienen una organización y una forma de ejecución diferentes, por lo que comparar únicamente el número de cores no permite deducir el rendimiento.

## Jerarquía de memoria y acceso

La memoria global de la GPU permite almacenar datos accesibles por los hilos del dispositivo. Los registros y la memoria compartida del SM son recursos más cercanos a las unidades de ejecución, pero su capacidad es limitada. También existen cachés; sus características dependen de la arquitectura.

El **ancho de banda de memoria** indica cuántos datos pueden transferirse por unidad de tiempo. Algunas GPU usan tecnologías como HBM, pero la elección depende del diseño de la GPU y del sistema, no únicamente de la CPU.

La **coalescencia** consiste en que los accesos de los hilos de un mismo warp puedan agruparse en transacciones de memoria eficientes. Por ejemplo, acceder a posiciones consecutivas y bien alineadas suele ser más favorable que acceder a posiciones muy separadas. El número de transacciones depende del patrón de direcciones y de la arquitectura; no es simplemente el número de caminos de acceso concurrentes.

# Arquitectura, capacidad de cómputo y compilación

La **arquitectura** describe la organización y las características del hardware. La **compute capability** es un número de versión que identifica capacidades de una GPU NVIDIA relevantes para CUDA, como funcionalidades e instrucciones disponibles. GPU de una misma familia arquitectónica pueden tener distintas capacidades y prestaciones; la capacidad tampoco mide por sí sola la velocidad.

Al compilar, hay que generar código compatible con la GPU donde se ejecutará. Una aplicación CUDA puede incluir código binario para uno o varios objetivos y también código intermedio **PTX**, que el controlador puede compilar para GPU compatibles en tiempo de ejecución. Las reglas de compatibilidad dependen del objetivo y del tipo de código: no debe suponerse que un binario generado para una GPU reciente funcione en una GPU antigua ni que todos los objetivos PTX tengan compatibilidad universal.

Por ejemplo, si se quiere ejecutar en una GPU antigua y otra reciente, hay que comprobar qué versiones de CUDA y qué objetivos de compilación admite cada una. El programa se compila para la GPU donde se va a **ejecutar**, que no tiene por qué estar en la máquina donde se compila.

## Evolución del hardware

Las mejoras pueden venir de más unidades de ejecución, unidades más capaces o especializadas, mayor ancho de banda de memoria, mejores cachés y otros cambios en la organización de los SM. Para aprovecharlas, el programa debe disponer de suficiente paralelismo y acceder a la memoria de forma eficiente.

## Referencias

- [Introducción a la plataforma CUDA y compatibilidad del código](https://docs.nvidia.com/cuda/cuda-programming-guide/01-introduction/cuda-platform.html).
- [Guía de programación CUDA C++](https://docs.nvidia.com/cuda/archive/12.9.2/cuda-c-programming-guide/index.html).
