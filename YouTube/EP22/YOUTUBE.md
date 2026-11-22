# Episodio 22

## Título
MPI EN C: normalizar un vector con Send y Recv | Punto a punto | EP 22

## Descripción
🍍 En el episodio 22 de Aprobar Computación Paralela retomamos la Tarea 2 y planteamos su versión con comunicaciones punto a punto en C: MPI_Send y MPI_Recv.

El objetivo es repartir un vector entre los procesos, obtener el máximo global, normalizar sus elementos y contar cuántos resultados superan 0.5. Finalmente, el proceso 0 reúne los bloques y guarda el vector normalizado en un archivo binario.

Trabajamos el reparto por bloques y la asignación del resto al proceso 0, las etiquetas de los mensajes, el intercambio de máximos locales y la suma de los conteos. También vemos cómo organizar manualmente la recogida de resultados que en la versión colectiva hacemos con MPI_Gatherv.

Para compilar y ejecutar en Linux o WSL con MPI instalado:
mpicc T2_PuntoAPunto.c -Wall -Wextra -o programa
mpirun -np 5 ./programa 100 salida.bin

El 5 indica los procesos y el 100 la dimensión del vector.

📌 Nota sobre el código: en el bucle de reparto inicial hay que avanzar pos con pos += num_elementos después de cada MPI_Send para que cada proceso reciba un bloque distinto.

En el episodio 21 tienes la versión con MPI_Bcast, MPI_Scatterv, MPI_Reduce y MPI_Gatherv para comparar ambas soluciones.

💬 ¿Qué versión te resulta más fácil de seguir: colectivas o punto a punto?
🔔 Suscríbete para seguir Aprobar Computación Paralela en C.

#MPI #ProgramacionEnC #ComputacionParalela
