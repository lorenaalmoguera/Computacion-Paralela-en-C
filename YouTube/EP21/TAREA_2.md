# TAREA 2

Se debe desarrollar un código paralelo usando **MPI** que procese un vector de **dimensión variable**, pasada por parámetro, de números enteros con valores entre **0 y 10000**.

El resultado será un vector del mismo tamaño de números de tipo **double**, normalizado entre **0 y 1**, que el **proceso 0** deberá guardar en un **fichero binario**.

Para realizar este procesamiento es necesario:

1. Calcular el **valor máximo** del vector.
2. Dividir todos los elementos por ese valor.
3. Calcular cuántos elementos del vector normalizado son **mayores que 0,5**.

Se realizarán dos versiones:

- Una versión con **comunicaciones punto a punto**.
- Otra versión con **comunicaciones colectivas**.
