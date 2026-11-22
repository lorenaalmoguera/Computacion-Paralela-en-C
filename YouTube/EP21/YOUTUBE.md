# Episodio 21

## Título
MPI EN C: Scatterv, Reduce y Gatherv | Normalizar un vector | EP 21

## Descripción
🍍 En el episodio 21 de Aprobar Computación Paralela resolvemos la Tarea 2 en C con comunicaciones colectivas de MPI.

Repartimos un vector de enteros entre varios procesos, calculamos su máximo global y normalizamos cada elemento dividiéndolo por ese máximo. Después contamos cuántos resultados son mayores que 0.5, reunimos el vector normalizado y lo guardamos en un archivo binario.

En este ejercicio trabajamos con:
• MPI_Bcast para compartir la dimensión y el máximo.
• MPI_Scatterv, send_counts y desplazamientos para repartir bloques de distinto tamaño.
• MPI_Reduce con MPI_MAX y MPI_SUM.
• MPI_Gatherv para reunir los resultados en el proceso 0.
• Memoria dinámica, tratamiento del resto y el caso de máximo igual a cero.

Para compilar y ejecutar en Linux o WSL con MPI instalado:
mpicc T2_Colectivas.c -Wall -Wextra -o programa
mpirun -np 5 ./programa 100 salida.bin

El 5 es el número de procesos y el 100 es el tamaño del vector: ¡recuerda pasar la dimensión!

En el episodio 22 resolvemos el mismo ejercicio utilizando MPI_Send y MPI_Recv.

💬 ¿Cómo repartirías los elementos si la dimensión no fuera divisible entre el número de procesos?
🔔 Suscríbete para seguir Aprobar Computación Paralela en C.

#MPI #ProgramacionEnC #ComputacionParalela
