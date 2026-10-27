# Arquitecturas de memoria compartida: OpenMP

Arquitecturas multicore forman parte de las arquitecturas de memoria compartida.

![Arquitectura multicore](arquitectura_multicore.png)

La herramienta software para explotar el paralelismo es OpenMP.

## Cosas a recordar

* cualquier desarrollo paralelo necesita **COMUNICACIONES y SINCRONIZACIONES**
* En memoria compartida las comunicaciones **NO UTILIZAN NINGÚN `SISTEMA DE COMUNICACIÓN` ENTRE COMPUTADORES**

La red de intercomunicación explícita que existía en MPI desaparece, pero esto no significa que las comunicaciones desaparezcan. Las comunicaciones en MPI se veían claramente porque estaban directamente relacionadas con las funciones de MPI. Yo hago las comunicaciones cuando explícitamente utilizo funciones de MPI. Teníamos el concepto de mensaje en estas comunicaciones en MPI, pero este en OpenMP ya no existe. A pesar de esto, seguirán existiendo las comunicaciones.

Aquí tiene muchísimo mas peso la sincronización. Esto se ve en por ejemplo las reducciones. Esperamos a obtener todos los resultados para sincronizar. En MPI no se hacia de forma explícita, se hacia de forma implicita. En OpenMP será de forma explícita.

Las comunicaciones se hacen a través de la memoria. La comunicación es compartir en este caso información, y lo voy a hacer siempre a través de la **memoria principal**. Solo existe un único espacio de memoria. Solo hay un mapa de memoria. Esto ha cambiado. 

Que la memoria sea compartida, es decir, única, y por lo tanto, hay un aspecto de MPI que vimos al principio que es cuando declarabamos una variable, cuantas variables de este tipo tengo?: Una por proceso, porque los procesos se estaban ejecutando cada uno en un procesador. Esto nos facilitaba, que estas variables que veiamos al principio (myrank), cada variable tenía un valor distinto. En una variable, el espacio es compartido. 

Una variable que tiene un sentido algorítmico clave, requiere que cada uno tenga un valor distinto. Esto significará que necesitaremos algunas variables que sean privadas. Es decir, que en los diferentes procesos que tengan en ejecución concurrentemente, pueda almacenar valores distintos en cada uno de ellos.

Una gestión de la memoria será una specto clave en el correcto funcionamiento y en la eficiencia, es decir, para reducir los tiempos ociosos. En openmp aparece un aspecto diferente, pero que está relacionado con MPI, y esto es LA GESTIÓN de la memoria.

Donde se hacen las comunicaciones en MPI? La gestión se hace en la memoria. Hay cosas en OpenMP que si se hacen, queda claro que no sabemos lo que estamos haciendo.

## Modelo de ejecución

En la segunda clase, hablamos del modelo de ejecución `nodo de cómputo multicore y multiprocesador`. Cuando yo ejecuto un proceso, se le asigna una zona de memoria.

Yo voy a empezar un programa paralelo, que comenzará como secuencial y en algún momento se convertirá en paralelo.

Dentro de mi ejecución secuencial, es deicr, dentro de mi zona de memoria que tengo asignada, y que porsupuesto puedo realizar las tareas de reserva dinámica.  La zona de memoria no es fija, es variable según la necesidad.

Cuando tenga mas de un proceso, en mi ejecución, estos procesos podrán acceder a toda la memoria de la ejecución, porque la memoria es compartida. Pero de alguna manera, nosotros tenemos que tener variables almacenadas en memoria privada, es decir, que sean propias de cada una de los procesos. En principio estas variables privadas no deberían de poder comunnicarse directamente, pero a través de la memoria principal si.

Esto serán mecanismos que tendremos que ver como se implementan.

## Aspecto que decidirá héctor si amplia o no:
El mapeo de los procesos en los cores puede ser realizado por SO o indicado por usuario.


Cuando estamos en MPI utilizamos nodos de cluster, y se especifica cuantos nodos queremos, y estos son los nodos que utilizamos.

Como decidimos en un sistema multicore? Porque lo que queremos es que en un hilo se ejecute un proceso, y en otro otro. Como gestionamos esto?

1. Dejamos la gestión al SO. El sistema operativo es el que decide el que mapea esos hilos: El sistema operativo lo decidirá en función de la carga de trabajo global y de cada proceso. Es decir, la gestión es dinámica. Son capaces de que si un proceso está en un determinado core, y la carga sube, pasar ese proceso a otro core. Es decir, aquí lo que hace es **balancear la carga**.
> Depende del SISTEMA OPERATIVO
2. Decidir cada hilo en que core lo queremos, e impedir al sistema operativo que lo mueva. Así tenemos todo el control del sistema. Cuando normalmente queremos las maximas eficiencias y seguimos queriendo usar el sistema computacional en exclusiva, habrá que hacer una planificación de este tipo. Sabemos lo que tenemos en el sistema y podremos planificar cada uno de los procesos en el sistema computacional y evitar los cambios de core a core.

Tenemos estas dos opciones.

1. La gestión al SO
2. Impedir, o solicitar al SO que no haga cambios de Core

Porque puedo querer yo decidir cada proceso en que core va? Para que: Los 4 cores comparten un chip de cpu. La memoria es muy importante para el rendimineto. 

Cuanto te perjudicaría decidir tú en que cores quieres que se ejecuten las cosas cuando no lo estas utilizando en exclusiva.

![Arquitectura multicore](arquitectura_multicore.png)