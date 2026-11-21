# Episodio 19

## Título

MPI EN C: ejercicio con Send y Recv | Aprobar Computación Paralela | EP:19

## Descripción

🍍 En el episodio 19 de Aprobar Computación Paralela desarrollamos la Tarea 1 en C con MPI: procesar un vector de 6000 enteros utilizando únicamente comunicaciones punto a punto.

El proceso 0 genera el vector con valores entre -10000 y 10000 y al menos 100 ceros. Repartimos el trabajo entre los procesos y reunimos sus resultados para calcular:

• La cantidad de números impares.
• La cantidad de números pares distintos de cero.
• La cantidad de números cuyas cifras suman 19, sin tener en cuenta el signo.

Durante el ejercicio trabajamos con MPI_Send y MPI_Recv, el reparto del vector por bloques, el tratamiento del resto de la división y la acumulación de los conteos en el proceso 0.

También representamos el reparto de datos con esquemas para entender qué parte del vector recibe cada proceso y cómo organizar las comunicaciones.

Si necesitas una introducción, en el episodio 18 revisamos varios ejemplos de código para familiarizarnos con MPI.

💬 ¿Cómo repartirías el vector si su tamaño no fuera divisible entre el número de procesos? Déjalo en los comentarios.
🔔 Suscríbete para seguir la serie Aprobar Computación Paralela en C.

#MPI #ProgramacionEnC #ComputacionParalela

## Miniatura

Archivo: miniatura_ep19.png

Texto principal: MPI EN C · PUNTO A PUNTO

Texto secundario: TAREA 1 · VECTOR DE 6000
