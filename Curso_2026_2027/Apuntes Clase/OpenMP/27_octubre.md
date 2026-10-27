# Arquitecturas de memoria compartida: OpenMP

## 1. Arquitectura multicore y memoria compartida

Una **arquitectura de memoria compartida** es un sistema en el que las unidades de ejecución comparten un espacio de memoria. Las arquitecturas multicore estudiadas en clase forman parte de este modelo.

![Arquitectura multicore](arquitectura_multicore.png)

**OpenMP** es una herramienta de programación que permite explotar el paralelismo en sistemas de memoria compartida mediante hilos de ejecución.

## 2. Comunicación y sincronización

Todo desarrollo paralelo necesita **comunicación y sincronización**, también cuando la memoria es compartida.

### Comunicación

La **comunicación** consiste en compartir información entre las unidades de ejecución.

- En **MPI**, los procesos tienen espacios de memoria separados y se comunican mediante mensajes, utilizando funciones explícitas de MPI.
- En **OpenMP**, los hilos comparten el espacio de memoria del proceso y se comunican leyendo y escribiendo datos compartidos. No se utiliza el modelo de intercambio de mensajes entre procesos de MPI.

Por tanto, desaparece el intercambio explícito de mensajes, pero **no desaparece la comunicación**: esta se realiza a través de la memoria compartida.

### Sincronización

La **sincronización** coordina la ejecución de los hilos para que las operaciones que dependen unas de otras se realicen en el orden necesario.

El profesor destaca su importancia en memoria compartida. Por ejemplo, para obtener el resultado de una reducción es necesario que estén disponibles las contribuciones que se van a combinar.

> La coordinación puede estar incorporada en una operación o requerir mecanismos explícitos. La comparación de clase no debe interpretarse como que toda sincronización en MPI sea implícita y toda sincronización en OpenMP sea explícita.

## 3. Variables compartidas y privadas

En MPI, cada proceso dispone de su propio espacio de memoria. Por eso, una variable como `myrank` puede tener un valor diferente en cada proceso.

En OpenMP hay que distinguir qué datos se comparten y cuáles necesita cada hilo por separado:

- Una **variable compartida** es un mismo dato al que pueden acceder varios hilos.
- Una **variable privada** tiene una copia independiente para cada hilo que la utiliza, de modo que cada uno puede almacenar su propio valor.

Esta distinción es necesaria cuando el algoritmo requiere que los hilos mantengan valores diferentes durante la ejecución concurrente. Para intercambiar los resultados de sus datos privados, los hilos necesitarán los mecanismos adecuados de comunicación y sincronización.

La **gestión de la memoria** es clave tanto para la corrección del programa como para su eficiencia. Hay que decidir qué información se comparte, qué información es privada y cómo se coordinan los accesos a los datos compartidos.

## 4. Modelo de ejecución

El modelo presentado en clase parte de un **nodo de cómputo multicore o multiprocesador**.

Al ejecutar un programa se crea un proceso con su espacio de memoria. Este puede realizar reservas dinámicas de memoria según sus necesidades.

La ejecución de un programa OpenMP comienza de forma **secuencial** y, al llegar a una región paralela, pasa a utilizar varios **hilos**. Estos pertenecen al mismo proceso y comparten su espacio de memoria, aunque pueden trabajar con variables privadas.

> En este contexto conviene distinguir **proceso** de **hilo**: el paralelismo de OpenMP se expresa mediante hilos dentro de un proceso.

Los mecanismos concretos para declarar y gestionar los datos compartidos y privados se estudiarán más adelante.

## 5. Asignación de hilos a núcleos

El **mapeo de hilos a núcleos** consiste en decidir en qué núcleo de la CPU se ejecuta cada hilo.

Héctor indicó que decidiría si ampliaba este tema y explicó las dos alternativas siguientes.

El profesor plantea dos posibilidades:

### Gestión por el sistema operativo

El **sistema operativo (SO)** decide dónde ejecutar los hilos en función de la carga global del sistema y de sus políticas de planificación.

La asignación puede cambiar durante la ejecución: el SO puede trasladar un hilo de un núcleo a otro para repartir la carga. El comportamiento concreto depende del sistema operativo.

### Asignación indicada por el usuario

El usuario puede indicar los núcleos en los que quiere que se ejecuten los hilos y restringir sus desplazamientos. Esta vinculación se conoce como **afinidad de hilos**.

Según lo comentado en clase, controlar esta asignación puede ser útil cuando se busca una eficiencia elevada y se dispone del sistema en exclusiva. Permite planificar dónde se ejecuta cada hilo teniendo en cuenta la organización de los núcleos y la memoria.

La asignación debe considerar la carga del sistema: fijar los hilos a determinados núcleos cuando el equipo se comparte con otras tareas puede perjudicar el rendimiento.

### Influencia de la arquitectura y del uso del sistema

El profesor relacionó esta decisión con la arquitectura multicore: en el ejemplo comentado, los cuatro núcleos comparten un mismo chip de CPU. La organización de los núcleos y de la memoria es importante para el rendimiento y debe tenerse en cuenta al decidir dónde ejecutar cada hilo.

También señaló que hay que considerar si el sistema se utiliza en exclusiva. Si se comparte con otras tareas, fijar la asignación limita la capacidad del SO para mover los hilos y equilibrar la carga, por lo que puede perjudicar el rendimiento.


## Que es OpenMP

Es un modelo basado en hilos o threads o procesos que deben usar caminos "hardware" diferentes de ejecución.

Los diferentes hilos ejecutarán el mismo código, pero realizarán diferentes funcionalidades.

En algunos casos tendré que especificar explícitamente de forma análoga lo que debe de hacer cada hilo.

Aquí aparecerán automatizadores, repartirá el trabajo sin yo explicitar exactamente como. De hecho a veces, yo no podré controlar lo que hace uno y lo que hace otro. El objetivo de los automatizadores será balancear la carga. Estamos en un sistema en el que es probable de que hayan mas procesos ejecutándose. Tenemos automatizadores que nos permitirán balancear la carga, es decir, nos permitirá mejorar la eficiencia ante situaciones de balanceo.

### Cosas que no vamos a usar
A partir de OpenMP 3.0 permite el trabajo con tareas "tasks". Esto no lo vamos a ver aquí. Veremos un automatizador que un poco puede vincularse al uso de tareas en versiones mas avanzadas (en código no).

No consideramos la utilización de HyperThreading.

Que es el HyperThreading
> Cada core se supone que puede ejcutar eficientemente mas de un hilo. No tiene recursos computacionales separados. Será un core. Tiene mecanismos para en determinadas circunstancias poder ejecutar mas de un hilo. Con lo cual si hacemos un alto uso de la ALU, por mucho que quiera no podremos obtener un buen rendimiento.

**ESTO NO LO VAMOS A HACER PORQUE NO NOS INTERESA EN HPC, APLICACIONES DE ALTO COSTE COMPUTACIONAL**


## Cosas que vamos a ver
* Modelo de ejecución
* Compilación y ejecución
* Qué es una directiva
* Qué es una cláusula
* Qué es una variable de entorno
* Memoria compartida y memoria Privada

# Modelo de Ejecución - Modelo FORK-JOIN

Primer aspecto relevante: Mi ejecución se inicia siempre de forma secuencial.

Yo solo tengo un código
Yo solo tengo un compilado

La ejecución va a ser en paralelo, pero en principio no lo vemos, mi ejecución no va a cambiar en secuencial. Si estoy en un sistema en linux, podré ejecutarlo en secuencial.

Por lo tanto, la ejecución empezará siempre con un único hilo.

Con loc ual si yo quiero ejecutar distintos recursos hardware, en algú momento tendré que generar por código y la herramienta de OpenMP tendré que generar HILOS en una determinada región, para ejecutar en paralelo un determinado código. En el momento de crearla decidiré.

Puedo tener una región paralela con 3 hilos y otra con 8.

Puedo tener una región paralela dentro de un bucle iterativo. 
Aunque sea la misma región paralela, en cada iteración puede ser un número de hilos de manera variable.

El número de hilos de cada región paralela se decide en ejecución, no en código. No tiene porque ser definido en código de manera literal.

Cuando termina una región paralela, no terminará la ejecución, simplemente continuará la ejecución secuencial


