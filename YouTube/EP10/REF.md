# Referencias para el episodio 9

## Apuntes de MPI vistos en clase

En estos apuntes está el contexto para entender el diseño paralelo, los procesos y las comunicaciones:

- [Arquitecturas distribuidas y modelo de MPI](../../Curso_2026_2027/Apuntes%20Clase/MPI/15_septiembre.md)
- [Procesos, rangos, comunicadores y mensajes](../../Curso_2026_2027/Apuntes%20Clase/MPI/22_septiembre.md)
- [Colectivas y comunicaciones](../../Curso_2026_2027/Apuntes%20Clase/MPI/29_septiembre_revisado.md)
- [Reparto del trabajo y descomposición de dominio](../../Curso_2026_2027/Apuntes%20Clase/MPI/concepto_de_repartir_trabajo.md)
- [Continuación de MPI y Scatterv](../../Curso_2026_2027/Apuntes%20Clase/MPI/06_octubre.md)

## Funciones colectivas y sus versiones con Send y Recv

- [Índice de funciones colectivas](../../Curso_2026_2027/Apuntes%20Clase/MPI/Funciones%20colectivas/README.md): resumen, resultado y enlaces a cada función.
- [Índice de implementaciones con Send y Recv](../../Curso_2026_2027/Apuntes%20Clase/MPI/Funciones%20colectivas/Send_Recv/README.md): explicación, diagramas y programas C independientes.

## Por qué practicar las colectivas con Send y Recv

Una función colectiva resume un patrón de comunicación entre procesos. Poder reconstruir ese patrón con `MPI_Send` y `MPI_Recv` ayuda a entender qué proceso aporta o recibe cada dato, cómo se divide un vector y qué operación local hace falta. También obliga a razonar sobre rangos, cantidades, etiquetas, orden de las comunicaciones y posibles esperas.

Esta comprensión resulta útil para estudiar y para resolver preguntas de examen que pidan explicar o representar el flujo de datos. No basta con memorizar la llamada colectiva: conviene poder dibujar quién envía a quién y qué resultado queda en cada proceso.

| Colectiva | Patrón punto a punto que conviene saber representar |
|---|---|
| `MPI_Bcast` | La raíz envía una copia del mismo bloque a cada proceso. |
| `MPI_Reduce` | Cada proceso aporta sus datos; la raíz reúne las contribuciones y aplica la operación de reducción. |
| `MPI_Allreduce` | Se hace una reducción y después se distribuye el resultado a todos. |
| `MPI_Gather` / `MPI_Gatherv` | Cada proceso envía su bloque a la raíz; con `Gatherv` pueden variar tamaños y desplazamientos. |
| `MPI_Scatter` / `MPI_Scatterv` | La raíz envía a cada proceso su parte; con `Scatterv` pueden variar tamaños y desplazamientos. |
| `MPI_Reduce_scatter` | Se reducen los vectores por posición y después se reparte el vector resultante. |
| `MPI_Scan` | Cada proceso obtiene un prefijo acumulado, respetando el orden de los rangos. |
| `MPI_Barrier` | Todos los procesos deben llegar al punto de sincronización antes de continuar. |

Al practicar, dibuja primero los datos de entrada y salida por rango; luego escribe los envíos y recepciones compatibles. Comprueba especialmente los casos con cantidades distintas, porque los desplazamientos se expresan en elementos del tipo MPI, no necesariamente en bytes.
