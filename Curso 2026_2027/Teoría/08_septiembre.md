# Introduccion 

> Todo lo que esté en este formato es lo que ha dicho el profesor en clase.

## Objetivos

1. Evaluar las prestaciones de un sistema paralelo en su conjunto software más hardware. 
2. Diseñar algoritmos paralelos y paralelizar códigos secuenciales ya existentes. 
3. Desarrollar programas para sistemas paralelos de memoria distribuida usando la librería 
especializada MPI. 
4. Desarrollar programas para sistemas paralelos de memoria compartida usando el paradigma 
OpenMP 
5. Conocer y la arquitectura y el paradigma de programación CUDA para GPUs en aplicaciones 
de propósito general.


## Conceptos a ver en esta asignatura:

> Como explotar Clusters -> (def clusters:) Conjunto de computadores conectados a través de una red de comunicación - Dividir el trabajo de una aplicación para diferentes recursos de cómputo.

> 1. Recurso de altas prestaciones: Librería MPI -> repartir trabjao, distribuir entre los distintos recursos de cómputo.

> Hay diferentes cores en el chip de un procesador, es decir procesadores independientes. Desde fuera es una unidad. Desde dentro son diferentes elementos de procesamiento llamados cores.

> Se decide como repartir el trabajo en esos cores.

> Sabiendo esto. 2. Recurso de altas prestaciones -> Librería OpenMP: Será la segunda parte de la asignatura. Tendremos ya conceptos de reparto de trabjo y veremos la diferencia entre esta y MPI.

> Arquitecturas cluster vs Arquitecturas de memoria distribuida.

> Las arquitecturas que nos encontramos suelen ser mixtas.

> 3. Tarjetas gráficas, GPUs. -> NVIDIA -> ManyCores


## Evaluación

* **Evaluación escrita (100%):** Un único examen final **escrito** que abarcará toda la materia de la asignatura, con duracion entre 2h30 y 3h.

## Recursos
Introducción a la programación paralela de Francisco Almeida Rodríguez, 
Domingo Giménez Cánovas, José Miguel Mantas Ruíz y Antonio Vidal Maciá 
Using MPI: portable parallel programming with the message-passing interface 
Gropp, William. Lusk, Ewing. Skjellum, Anthony.Edit: Cambridge, MA: MIT 
Press, 1999 
Using OpenMP: portable shared memory parallel programming. Chapman, 
Barbara, Jost, Gabriele. Pas, Ruud van der. The MIT Press, 2008. 
CUDA by Example: An Introduction to General-Purpose GPU 
Programming 
de Jason Sanders y Edward Kandrot 
Fundamentos de los computadores. Pedro de Miguel Anasagasti. Paraninfo. 
Estructura de computadores. Angulo Usategui, José María. Paraninfo 1996 
Arquitectura de computadores. Ortega Lopera, Julio - Prieto Espinosa, Alberto 
Anguita López, Mancia. Thomson. 
Organización y Arquitectura de Computadores Diseño para Optimizar 
Prestaciones de William Stallings, Antonio Cañas Vargas y Alberto Prieto 
Espinosa 
MPI: https://www.mpi-forum.org/ 
OPENMP: http://openmp.org/wp/ 
CUDA: http://www.nvidia.es/object/cuda-parallel-computing-es.html 
** Mucha información adicional en web e IAs generativas**

# Explicación P0_T1 algunas preguntas en clases
* *Suponiendo que el fichero almacena enteros. ¿Cuál es la cantidad máxima de enteros V1 y V2?* -> Nos debemos hacer la pregunta: ¿Cuánto ocupa un entero?

* *¿Cuá sería el tamaño del fichero, si es que puede determinarse, si en lugar de ser binario fuera de tipo texto?* -> Si. Medimos por carácteres. Sabemos de que número se trata por ASCII. Y podríamos saber cuando terminar un número por los delimitadores.

* *Si el fichero de 400 bytes es de tipo texto, ¿Podemos determinar únicamente a partir de su tamaño cuántos enteros contiene?* -> unicamente si tenemos signos negativos y positivos y si hay claros separadores.

> 200 maximo, como maximo 100 entre los dos.

* ¿Cómo podemos saber exactamente cuántos elementos tenemos que leer? -> por argumento / cli o pasándo un fichero.

* ¿Qué tipo de memoria es preferible utilizar, estática o dinámica? -> dinámica, para poder reservar según la memoria que necesite.

* Si el número de elementos a leer se almacena en la variable tam, ¿qué diferencias existen entre realizar la reserva de las siguientes formas: a) `int v[tam]` y b) int `*v = malloc(tam*sizeofi(int));`? -> la memoria dinámica continuará existiendo hasta que se haga un free. hay también una diferencia en la zona en la que se guarda.
# Día