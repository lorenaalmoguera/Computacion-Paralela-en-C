# ESTE REPOSITORIO HA SIDO CREADO CON LA INTENCIÓN DE AYUDAR AL ESTUDIANTADO DE LA UMH A APROBAR COMPUTACIÓN PARALELA.
ESTE REPOSITORIO SERÁ ACTUALIZADO A MEDIDA DE QUE VAYA YO ESTUDIANDO TAMBIÉN. LA IDEA ES PRESENTARME EN ENERO.
## ESTARÁ DIVIDIDO EN VARIAS SECCIONES.

1.- SECUENCIAL.
2.- MPI.
3.- OPENMP.

**NO VOY A TOCAR CUDA.**

EN VERANO... EL PROFESOR **CIERRA** EL SERVIDOR ONLINE... ASÍ QUE DEBÉIS HACER UNA DE LAS SIGUIENTES COSAS:
- INSTALAROS UNA **MÁQUINA VIRTUAL** CON LINUX (*para no joderos el PC*) Y MPI/OPENM EN LA MÁQUINA VIRTUAL.
- INSTALAROS MPI/OPENMP EN VUESTRO SISTEMA OPERATIVO DIRECTAMENTE
(yo personalmente he hecho lo segundo, ya que la máquina virtual iba demasiado lenta...XD)

La asignatura tiene una CARGA DE TRABAJO GRANDE y la gente no la suele aprobar a la primera pero hay casos en los que si.
NO RECOMIENDO pillarla junto a otras asignaturas difíciles o con una carga de trabajo grande. Si podéis, pilladla el primer año que no tengáis muchas asignaturas chungas.

### RECOMIENDO ADEMÁS LEER EL SIGUIENTE TEXTO:

Para aprobar computación paralela se deben llevar los siguientes conceptos al día:

## 0- GESTIÓN DE FICHEROS (BINARIOS Y NORMALES)
    * la gestión de ficheros (lectura y escritura) será necesaria para abordar la asignatura
## 1- GESTIÓN DE MEMORIA (PUNTEROS)
    * la gestión de memoria es lo que más problemas suele dar a la hora de realizar el examen práctico obligatorio.
    * si no llevas la gestión de memoria bien NO empieces a parelizar. realiza ejerciciós en secuencial hasta que la lleves PERFECTA.
## 2- GESTIÓN DE MATRICES Y VECTORES
    * con la gestión de matrices me refiero a operaciones de y dentro de matrices... como por ejemplo:
        1.- Multiplicación de matrices
        2.- Calculo de media de matriz (de una fila, de una columna (si aplica), la matriz/vector entera/0, de los vecinos de un elemento, etc...)
        3.- Desviación media de una matriz
        4.- Intercambio de filas / elementos de un/a matriz / vector
        5.- etc...
    * si no se llevan los conceptos de arriba bien, será IMPOSIBLE aprobar la asignatura así que una vez más se debe llevar la gestión de memoria (para poder trabajar con las matrices y vectores) y la gestión de vectores y matrices.
## 3.- CONCEPTOS DE MPI y OPENMP
    * se DEBEN llevar estos conceptos al día ya sea para los ejercicios de traza o los ejercicios prácticos.
    * siendo completamente sinceros... paralelizar en la asignatura NO es lo más complicado... se difículta ÚNICAMENTE si NO se llevan al día los conceptos anteriores... por lo que DEBÉIS priorizar eso antes de empezar a paralelizar, ya que lo único difícil es:
        1.- el reparto de trabajo, si no lleváis bien las mates
        2.- una vez más... la gestión de memoria.

# PRÁCTICAS:

Antes del cambio de la asignatura las prácticas eran obligaotrias. No sé si al final habrá cambio o no, pero recomiendo hacer dichas prácticas con tiempo y recomiendo utilizar chatgpt LO MÍNIMO POSIBLE. De verdad, sé que es complicado, pero solo empezad a usarlo únicamente si empezáis a ir muy mal de tiempo o algo... O si hay algo que no entendéis.

* EL USO DE UN COMANDO/CONCEPTO DE MPI O OPENMP NO DADO EN LA ASIGNATURA CONLLEVARÁ A UN SUSPENSO AUTOMÁTICO.

## PRACTICAS EN MPI
El profesor siempre mirará los siguientes conceptos. En algunos casos si ocurren, la práctica será un NO APTO.

1. Copia de memoria innecesaria
2. Comunicaciones colectivas no óptimas
3. Uso de funciones no vistas
4. Uso excesivo de funciones puede impedir eficiencia secuencial
5. Código sin paralelizar
6. Reservas de memoria fuera de sitio
7. Código repetido
8. Comunicaciones no pedidas contabilizadas
9. Exceso de código no usado
10. Nombres ficheros muy largos
11. Comunicaciones innecesarias

NO APTO 100%

100. Faltan sentencias de compilación
101. No compila

## PRACTICAS OPENMP

El profesor siempre mirará los siguientes conceptos. En algunos casos si ocurren, la práctica será un NO APTO.
0.- Copias de memoria innecesarias
1.- Alguna(s) cláusula(s) con valor por defecto
2.- Exceso de barreras
3.- Se considera tamaño de filas divisible entre número de hilos
4.- No se ha incluido el parámetro con el número de hilos
5.- Fases sin paralelizar
6.- Petición de parámetros por HID
7.- Alguna funcionalidad no correcta
8.- Cláusulas no vistas en clase
9.- No uso de constructores combinados (cuando eran recomendables)
10.- Directivas fuera de región paralela (que deben estar dentro)

# EXAMEN PRÁCTICO A ORDENADOR:
En 2025/26 y años anteriores suspender el examen online de computación paralela conlleva obtener la restricción de nota del 90%... Como bien sabéis esto es lo que dificulta aún más la asignatura.

Os aviso ya que llevar ESQUELETOS preparados no os va a servir de nada. Yo lleve 30 esqueletos preparados y me encontré con que el profesor te daba una plantilla con la gestión de memoria hecha como EL la hacía de normal... Por lo que...
    1.- Si no llevas la gestión de memoria y de vectores / matrices perfecta vas a suspender.
Debéis machacar mucho lo secuencial para poder aprobar la asignatura y llevarlo absolutamente todo SIN DUDAS.


# EXAMEN ESCRITO
Para aprobar el examen se debe sacar un % mínimo que el profesor considere en cada uno de los apartados que nombraré a continuación.
    1.- preguntas de teoria
    2.- trazas a realizar a mano
    3.- ejercicio práctico a mano de MPI
    4.- ejercicio práctico a mano de OpenMP

* Para abordar la teoría de la asignatura recomiendo que os hagáis un NOTEBOOKLM ya que ayuda muchísimo a la hora de realizar la asignatura.
* Para abordar las trazas a mano recomiendo usar los ejercicios del profesor.  Si le decis a la IA que os haga trazas similares, y los entendéis, esto en el examen lo sabréis hacer.
* Para abordar el ejercicio práctico de MPI recomiendo conocer el enunciado del exámen práctico de ese mismo año. ENTENDED EL EXAMEN y hacedlo vosotros mismos a mano. Además, si habéis hecho todo lo que os he dicho antes... machacar el secuencial, decidle a vuestra IA que os tire enunciados aleatorios y poder hacerlos etc... aprobaréis.
* Para abordar el ejercicio práctico de OPENMP recomiendo conocer el enunciado del exámen práctico de ese mismo año. ENTENDED EL EXAMEN y hacedlo vosotros mismos a mano. Además, si habéis hecho todo lo que os he dicho antes... machacar el secuencial, decidle a vuestra IA que os tire enunciados aleatorios y poder hacerlos etc... aprobaréis.



