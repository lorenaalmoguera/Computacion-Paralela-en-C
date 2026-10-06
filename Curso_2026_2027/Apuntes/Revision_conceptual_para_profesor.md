# Revisión conceptual para consultar al profesor

Fecha: 6 de octubre de 2026.

Este informe recoge posibles errores y ambigüedades de los apuntes del repositorio, incluidas las explicaciones añadidas durante su revisión. No atribuye estos errores al profesor: pueden proceder de la transcripción o de los ejemplos. Las propuestas se presentan para su confirmación docente. Los textos originales de Markdown pueden consultarse en el historial de Git; los PDF originales se conservan.

Las referencias de archivo son relativas a Curso_2026_2027. Cuando se describe la idea original se trata de una paráfrasis, no de una cita literal.

## Errores conceptuales y corrección propuesta

1. **Scan: cantidad de operandos distinta en cada proceso.** Archivo: Apuntes Clase/MPI/06_octubre.md. Cada proceso aporta el mismo número de elementos, count. Lo que cambia es el prefijo reducido: el proceso de rango r obtiene la combinación de las contribuciones de 0 a r. Ejemplo con un entero por proceso, entradas [1,2,3,4]: resultados [1,3,6,10]. Con vectores, se aplica elemento a elemento. Referencia: [MPI_Scan](https://docs.open-mpi.org/en/main/man-openmpi/man3/MPI_Scan.3.html).

2. **Gather: excluir la aportación de la raíz.** Archivo: Apuntes Clase/MPI/06_octubre.md. La raíz también participa y aporta datos. En una implementación manual puede copiar su bloque localmente, sin enviarse un mensaje. El resultado se coloca por orden de rango y recvcount indica elementos por proceso, no el total. Referencia: [MPI_Gather](https://docs.open-mpi.org/en/main/man-openmpi/man3/MPI_Gather.3.html).

3. **MPI_ANY_SOURCE ordena automáticamente las aportaciones o garantiza más rapidez.** Archivo: Apuntes Clase/MPI/06_octubre.md. El comodín acepta cualquier origen compatible. Hay que consultar status.MPI_SOURCE y colocar los datos en su posición si se desea orden por rango. El rendimiento depende del caso. Referencia: [MPI_Recv](https://docs.open-mpi.org/en/main/man-openmpi/man3/MPI_Recv.3.html).

4. **Gatherv exige varias reservas porque los envíos tienen tamaños distintos.** Archivo: Apuntes Clase/MPI/06_octubre.md. La raíz puede usar una única reserva. recvcounts determina cuántos elementos recibe de cada rango y displs dónde empieza cada bloque, en unidades de la extensión de recvtype. Para MPI_INT, counts=[4,2,6,3] y displs=[0,4,6,12] requieren 15 enteros. Referencia: [MPI_Gatherv](https://docs.open-mpi.org/en/main/man-openmpi/man3/MPI_Gatherv.3.html).

5. **Reservar sum(recvcounts) siempre basta para Gatherv.** Archivo: Apuntes Clase/MPI/06_octubre.md. Para MPI_INT con desplazamientos no negativos, hay que cubrir hasta max(displs[i]+recvcounts[i]). Con counts=[2,2,2] y displs=[0,4,6], se necesitan 8 posiciones aunque solo se reciban 6 enteros. La fórmula necesita más cuidado con tipos derivados. Referencia: MPI_Gatherv, enlace anterior.

6. **Los huecos de Gatherv se rellenan con cero o pueden solaparse los bloques.** Archivo: Apuntes Clase/MPI/06_octubre.md. Los huecos conservan el contenido previo; hay que inicializarlos para mostrar ceros. No es válido especificar escrituras solapadas en el búfer receptor ni escribir fuera de la reserva. Referencia: MPI_Gatherv, enlace anterior.

7. **Una reserva de memoria determina el número de mensajes.** Archivo: Apuntes Clase/MPI/06_octubre.md. La reserva y las comunicaciones son decisiones distintas. Un bloque con dos vectores contiguos puede enviarse en una llamada con 2*tam elementos o en dos llamadas de tam elementos. Tener dos punteros no crea dos mensajes.

8. **Recv exige exactamente el mismo count que Send.** Archivos: Apuntes Clase/MPI/15_septiembre.md, 22_septiembre.md, 29_septiembre.pdf y Apuntes/03_mpi.md. count en Recv expresa capacidad máxima. Send de 5 MPI_INT y Recv de capacidad 10 MPI_INT es válido si el búfer tiene espacio; capacidad 3 provoca truncamiento. Los tipos deben ser compatibles. MPI_Get_count permite conocer la cantidad recibida. Referencia: MPI_Recv.

9. **El tipo y el tamaño seleccionan qué mensaje recibe MPI_Recv.** Archivos: Apuntes Clase/MPI/22_septiembre.md, 29_septiembre.pdf y Apuntes/03_mpi.md. El emparejamiento se determina por origen, destino, etiqueta y comunicador/contexto. El tipo y la capacidad son condiciones de transferencia correcta, no filtros para elegir entre mensajes. MPI_ANY_SOURCE flexibiliza el origen, no elimina las demás condiciones. Referencia: MPI_Recv.

10. **count representa bytes en cualquier llamada MPI.** Archivo: Apuntes Clase/MPI/22_septiembre.md. Representa elementos del tipo MPI indicado. count=10 con MPI_INT significa diez enteros; MPI_BYTE permite expresar bytes. Referencia: [MPI_Send](https://docs.open-mpi.org/en/main/man-openmpi/man3/MPI_Send.3.html).

11. **Un proceso no puede enviarse mensajes a sí mismo.** Archivo: Apuntes Clase/MPI/22_septiembre.md. Es legal. Sin embargo, hacer un MPI_Send bloqueante a uno mismo antes de publicar la recepción puede bloquearse si depende de un búfer que MPI no garantiza. Hay que diseñar correctamente el orden o usar alternativas apropiadas como MPI_Sendrecv. Referencia: [MPI_Sendrecv](https://docs.open-mpi.org/en/main/man-openmpi/man3/MPI_Sendrecv.3.html).

12. **MPI_Send vuelve únicamente cuando el receptor ya recibió o procesó todo.** Archivos: Apuntes Clase/MPI/15_septiembre.md y 29_septiembre.pdf. Al completarse el envío estándar puede reutilizarse el búfer del emisor, pero eso no acredita que el receptor haya terminado de recibir o procesar. El comportamiento puede depender del uso de almacenamiento interno. Referencia: MPI_Send.

13. **MPI_Finalize vacía o resuelve mensajes pendientes y sustituye una barrera.** Archivos: Apuntes Clase/MPI/22_septiembre.md y 29_septiembre.pdf. Antes de finalizar, el programa debe completar las operaciones y los emparejamientos necesarios. Finalize es colectiva, pero no debe usarse como remedio de comunicaciones incorrectas ni como sustituto general de MPI_Barrier. Referencia: [Finalización en el estándar MPI](https://www.mpi-forum.org/docs/mpi-5.0/mpi50-report/node270.htm).

14. **Reduce se llama solo en la raíz o reduce cualquier vector a un único escalar.** Archivo: Apuntes Clase/MPI/29_septiembre.pdf. Todos los procesos del comunicador llaman a la operación. Con count=n, produce n resultados, combinando cada posición entre procesos. Para sumar además las posiciones de un vector, se calcula primero una suma local y luego se reduce un escalar. Referencia: [MPI_Reduce](https://docs.open-mpi.org/en/main/man-openmpi/man3/MPI_Reduce.3.html).

15. **MINLOC significa máximo y localización; basta una estructura C cualquiera.** Archivo: Apuntes Clase/MPI/29_septiembre.pdf. MINLOC devuelve mínimo y su índice asociado; MAXLOC devuelve máximo e índice. Hay que utilizar un tipo de pareja compatible, por ejemplo MPI_DOUBLE_INT para double e int. No basta definir una estructura sin indicar correctamente su tipo MPI. Referencia: MPI_Reduce.

16. **MPI_Comm_size entrega valores distintos como 0,1,2,3 a los procesos.** Archivo: Apuntes Clase/MPI/22_septiembre.md. size devuelve el número de miembros: con cuatro procesos, todos obtienen 4. rank devuelve 0,1,2,3 respectivamente. Referencia: [MPI_Comm_size](https://docs.open-mpi.org/en/main/man-openmpi/man3/MPI_Comm_size.3.html).

17. **MPI no tiene organismo responsable o no ofrece control de errores.** Archivo: Apuntes Clase/MPI/15_septiembre.md. El estándar lo desarrolla MPI Forum. Las llamadas tienen mecanismos de error y los comunicadores manejadores; el comportamiento por defecto puede abortar la ejecución. Un destino inválido no debe considerarse una comunicación que funciona por suerte. Referencias: [MPI Forum](https://www.mpi-forum.org/docs/) y MPI_Send.

18. **MPI no aprovecha varios núcleos de una misma máquina o exige un proceso por procesador.** Archivo: Apuntes Clase/MPI/15_septiembre.md. Pueden ejecutarse varios procesos MPI en un equipo multinúcleo. La asignación a núcleos y la cantidad de procesos se configuran; un proceso por núcleo puede ser una recomendación del ejercicio, no una exigencia universal de MPI. MPI y OpenMP también pueden combinarse.

19. **La memoria dinámica se reserva en la pila.** Archivo: Apuntes Clase/UD1/11_septiembre.pdf, página 1. malloc proporciona almacenamiento de duración asignada, normalmente gestionado en el heap. En C no conviene equiparar esta memoria con la pila de variables automáticas. realloc puede moverla: hay que comprobar el resultado y recalcular los punteros al bloque si cambia.

20. **Un cauce de tres etapas termina tres instrucciones por ciclo.** Archivo: Apuntes Clase/UD1/11_septiembre.pdf, página 1. Un cauce escalar ideal de tres etapas puede tener tres instrucciones en vuelo, pero normalmente completa una por ciclo una vez lleno. Terminar varias por ciclo requiere capacidad adicional de emisión/ejecución; tampoco basta con que no haya saltos.

21. **La afinidad suma las cachés L2 y garantiza más velocidad.** Archivo: Apuntes Clase/UD1/11_septiembre.pdf, página 1. La afinidad restringe las CPU donde puede ejecutarse un hilo/proceso. Puede favorecer la localidad o reducir migraciones, pero no fusiona cachés ni garantiza una mejora. Referencia: [CPUSETS, documentación del núcleo Linux](https://www.kernel.org/doc/html/latest/admin-guide/cgroup-v1/cpusets.html).

22. **Contar valores pares equivale a sumar sus valores.** Archivo: Apuntes/Study_Guide_Computacion_Paralela_2026-2027.md, apartado Leer y reducir. Contar [2,4,7] produce 2; sumar sus valores pares produce 6. Para contar se reducen contadores; para sumar se reducen sumas locales. Esta imprecisión también estaba en la guía elaborada durante la revisión.

23. **El tamaño en bytes de un fichero de texto determina cuántos enteros contiene.** Archivo: Apuntes Clase/UD1/08_septiembre.md. Los números tienen longitudes distintas y separadores. Hay que analizar el contenido o disponer de un formato/metadatos que indiquen la cantidad. Un fichero binario de enteros de tamaño fijo es un caso diferente.

## Afirmaciones que necesitan contexto, no deben marcarse automáticamente como falsas

24. **El resultado paralelo debe ser exactamente idéntico al secuencial.** Archivo: Apuntes Clase/MPI/15_septiembre.md. Puede exigirse igualdad exacta en ejercicios con enteros y operaciones válidas. En coma flotante, cambiar el orden de sumas puede cambiar el redondeo; conviene especificar tolerancias y reproducibilidad.

25. **Siempre hace falta un coordinador, toda comunicación sincroniza a todos y la raíz guarda toda la entrada.** Archivo: Apuntes Clase/MPI/15_septiembre.md. Son posibles diseños de ejemplos, no reglas generales. Hay tareas independientes; las comunicaciones coordinan participantes concretos sin implicar siempre una barrera global. Una reducción de parciales no exige almacenar todos los datos originales en la raíz.

26. **Allreduce siempre ejecuta internamente Reduce y después Bcast; Scan siempre sigue una cadena concreta de mensajes.** Archivos: Apuntes Clase/MPI/06_octubre.md y 29_septiembre.pdf. Son modelos didácticos o implementaciones posibles. La colectiva garantiza el resultado definido, no ese algoritmo interno.

27. **Los mensajes MPI siempre requieren datos contiguos.** Archivos: Apuntes Clase/MPI/15_septiembre.md y Apuntes/03_mpi.md. Es una simplificación válida para los ejemplos con tipos básicos. Los tipos derivados permiten describir posiciones no contiguas de memoria.

28. **Fortran siempre es más eficiente que C; Ethernet no pierde prestaciones al añadir conexiones; PGAS no es óptimo; el reparto funcional nunca escala.** Archivos: Apuntes Clase/MPI/15_septiembre.md y concepto_de_repartir_trabajo.md. Son conclusiones dependientes del programa, compilador, arquitectura, topología, carga y reparto. Conviene formularlas como casos o condiciones concretas.

29. **La eficiencia mide directamente ocupación de CPU; la escalabilidad no se cuantifica.** Archivos: Apuntes Clase/UD1/11_septiembre.pdf y Apuntes/01_fundamentos.md. La eficiencia E=S/p es una razón de aceleración por recurso, no una lectura directa del monitor de CPU. La escalabilidad puede estudiarse con tiempos, aceleración, eficiencia y tamaños de problema; hay que indicar el experimento y la referencia secuencial.

30. **Concurrencia, paralelismo y multitarea equivalen entre sí.** Archivo: Apuntes Clase/UD1/11_septiembre.pdf. Concurrencia describe tareas cuyo progreso se solapa; paralelismo implica ejecución simultánea; multitarea es la gestión de varias tareas y puede existir con un solo núcleo.

## Errores de código, nombres y ejemplos, separados de los conceptos

31. **Reserva incorrecta:** int *datos = *(int)malloc(...), en 06_octubre.md. En C: int *datos = malloc(2 * tam * sizeof *datos); con stdlib.h, tamaño válido y comprobación de NULL. Después v1=datos y v2=datos+tam. Se libera únicamente datos. Evitar desbordamiento al calcular tamaños.

32. **Firma incompleta de Gatherv:** falta sendtype en 06_octubre.md. Firma: MPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm).

33. **Confundir retorno y parámetro de salida:** int myrank = MPI_Comm_rank(..., &myrank), en 22_septiembre.md, acaba asignando a myrank el código de retorno. Usar int myrank; int ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

34. **Nombres de C y tipos:** en 22_septiembre.md y 29_septiembre.pdf, usar MPI_Comm_size y MPI_Comm_rank respetando mayúsculas. Para int de C, MPI_INT; MPI_INTEGER corresponde al enlace Fortran.

35. **Otros ejemplos:** declaración duplicada de ierr, nprocess/nproces inconsistentes, printf sin el argumento indicado, vector enviado sin inicializar, sizeofi(int) y decimal 47645,3. Corregir nombres/argumentos, inicializar datos, usar sizeof(int) y 47645.3. La coma es un operador de C, no el separador decimal.

36. **Ejemplos numéricos incoherentes:** en 06_octubre.md había un dibujo con tres procesos pero cuatro desplazamientos/contribuciones. El número de entradas de counts/displs y las aportaciones deben corresponder al comunicador usado.

## Preguntas sugeridas al profesor

- ¿Estas correcciones reflejan correctamente el estándar y lo que se pretendía explicar en clase?
- ¿Qué afirmaciones eran simplificaciones deliberadas para los ejercicios, especialmente igualdad de count, coordinador y datos contiguos?
- ¿En las prácticas se exige igualdad exacta o tolerancia cuando hay coma flotante?
- ¿Podemos mantener las implementaciones con Send/Recv como ejemplos didácticos, indicando que no reproducen necesariamente el algoritmo interno de la biblioteca?

No se han considerado errores los ejemplos de bloqueo deliberados ni se ha revisado aquí la vigencia de porcentajes de evaluación o ediciones bibliográficas.

