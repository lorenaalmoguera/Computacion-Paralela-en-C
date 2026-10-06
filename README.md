# Computación Paralela en C

Material de apoyo para estudiar Computación Paralela en la UMH: programación secuencial en C, MPI y OpenMP. Incluye apuntes, enunciados, ejemplos de clase, prácticas y archivos de datos. Se amplía conforme avanzamos en la asignatura.

Hay material del curso **2026–2027** y prácticas de **2025–2026**. La numeración de las prácticas depende del curso: una P0-3 antigua no tiene por qué corresponder a la P0_T3 actual. El repositorio no incluye una sección de CUDA.

## Navegación

- [Por dónde empezar](#por-donde-empezar)
- [Mapa del repositorio](#mapa-del-repositorio)
- [Curso 2026–2027](#curso-2026-2027)
- [Programación secuencial y prácticas anteriores](#secuencial)
- [MPI: ejemplos, colectivas y prácticas](#mpi)
- [OpenMP: ejemplos y prácticas](#openmp)
- [Material de YouTube](#youtube)
- [Datos, formatos y herramientas](#datos-y-herramientas)
- [Compilar y ejecutar](#compilar)
- [VS Code local con MPI en WSL](#vscode-wsl)
- [Consejos de estudio y exámenes](#consejos)
- [Colaboradores](#colaboradores)

<a id="por-donde-empezar"></a>

## Por dónde empezar

1. Lee [los fundamentos](Curso_2026_2027/Apuntes/01_fundamentos.md) y [los apuntes de C secuencial](Curso_2026_2027/Apuntes/02_programacion_secuencial.md). Practica ficheros binarios, punteros, reservas de memoria, vectores y matrices.
2. Resuelve [P0_T1](Curso_2026_2027/Practica/Secuencial/P0-1/README.md), [P0_T2](Curso_2026_2027/Practica/Secuencial/P0-2/enunciado_P0_T2.md) y [P0_T3](Curso_2026_2027/Practica/Secuencial/P0-3/enunciado_P0_T3.md).
3. Continúa con [los apuntes de MPI](Curso_2026_2027/Apuntes/03_mpi.md), el reparto del trabajo y los mensajes Send/Recv.
4. Estudia [los diagramas de las colectivas](Curso_2026_2027/Apuntes%20Clase/MPI/Funciones%20colectivas/README.md) y compara cada operación con [su implementación punto a punto](Curso_2026_2027/Apuntes%20Clase/MPI/Funciones%20colectivas/Send_Recv/README.md).
5. Trabaja los ejemplos y prácticas de MPI y OpenMP. Antes de paralelizar, comprueba el resultado de tu versión secuencial.
6. Utiliza [la guía de estudio](Curso_2026_2027/Apuntes/Study_Guide_Computacion_Paralela_2026-2027.md) para repasar y preparar preguntas.

<a id="mapa-del-repositorio"></a>

## Mapa del repositorio

| Carpeta | Contenido | Uso |
|---|---|---|
| [Curso_2026_2027](Curso_2026_2027) | Apuntes organizados, notas de clase y prácticas del curso actual. | Seguir las clases y empezar a estudiar. |
| [SECUENCIAL](SECUENCIAL) | Ejercicios propios y prácticas de 2025–26 con enunciados y datos. | Afianzar C antes de paralelizar. |
| [MPI](MPI) | Ejemplos de clase, scripts y material de prácticas con memoria distribuida. | Estudiar procesos y comunicación. |
| [OpenMP](OpenMP) | Ejemplos de directivas, scripts y material de prácticas con memoria compartida. | Estudiar hilos y reparto de trabajo. |
| [YouTube](YouTube) | Material agrupado por episodios EP1–EP10. | Acompañar las explicaciones con código y presentaciones. |
| [.vscode](.vscode) | Configuración del editor, tarea de GCC y script de IntelliSense para MPI. | Preparar el entorno local. |

<a id="curso-2026-2027"></a>

## Curso 2026–2027

### Apuntes organizados

El [índice de apuntes](Curso_2026_2027/Apuntes/README.md) organiza tres bloques:

| Documento | Contenido |
|---|---|
| [01_fundamentos.md](Curso_2026_2027/Apuntes/01_fundamentos.md) | Fundamentos de arquitectura y paralelismo. |
| [02_programacion_secuencial.md](Curso_2026_2027/Apuntes/02_programacion_secuencial.md) | Programación secuencial en C, memoria y tratamiento de datos. |
| [03_mpi.md](Curso_2026_2027/Apuntes/03_mpi.md) | Procesos, comunicadores, mensajes, colectivas y reparto del trabajo. |
| [Guía de estudio en Markdown](Curso_2026_2027/Apuntes/Study_Guide_Computacion_Paralela_2026-2027.md) | Repaso de conceptos, errores frecuentes y preguntas de estudio. |
| [Guía de estudio en Word](Curso_2026_2027/Apuntes/Study_Guide_Computacion_Paralela_2026-2027.docx) | Versión de la guía en formato DOCX. |

Estos apuntes tienen un alcance propio; las notas de clase pueden contener temas posteriores que aún no estén incorporados al resumen.

### Apuntes de clase

| Ubicación | Contenido |
|---|---|
| [UD1](Curso_2026_2027/Apuntes%20Clase/UD1) | Notas del 8 de septiembre y PDF del 11 de septiembre. |
| [MPI](Curso_2026_2027/Apuntes%20Clase/MPI) | Notas del 15 y 22 de septiembre, PDF del 29 de septiembre y notas del 6 de octubre. |
| [Concepto de repartir trabajo](Curso_2026_2027/Apuntes%20Clase/MPI/concepto_de_repartir_trabajo.md) | Explicación del reparto y la descomposición del trabajo, acompañada por las imágenes descomp_dominio*.png. |
| [Funciones colectivas](Curso_2026_2027/Apuntes%20Clase/MPI/Funciones%20colectivas/README.md) | Diagramas Mermaid, funcionamiento, ejemplos en C y resultados esperados. |
| [Send_Recv](Curso_2026_2027/Apuntes%20Clase/MPI/Funciones%20colectivas/Send_Recv/README.md) | Las diez operaciones de los diagramas reconstruidas con mensajes punto a punto. Cada .c es independiente y muestra toda la implementación dentro de main. |

Los diagramas Mermaid se visualizan en visores compatibles, incluido GitHub. Las páginas también describen los resultados en texto.

### Prácticas secuenciales actuales

| Práctica | Contenido |
|---|---|
| [P0-1](Curso_2026_2027/Practica/Secuencial/P0-1) | Dos vectores en un fichero binario; lectura, tamaños y comparación. Incluye README con preguntas y respuestas, y versiones con punteros y con índices. |
| [P0-2](Curso_2026_2027/Practica/Secuencial/P0-2) | Memoria dinámica para dos vectores y búsqueda del máximo; variantes con una reserva o con dos reservas. |
| [P0-3](Curso_2026_2027/Practica/Secuencial/P0-3) | Punteros y aritmética de punteros, con accesos mediante distintos tipos. Incluye enunciado y código. |

Cada carpeta contiene datos P0_T1_datos_random.bin y un ejecutable main.exe. Compila el código fuente para tu entorno. Los enunciados incluyen cuestiones sobre tamaños y accesos a memoria: comprueba los límites del fichero antes de interpretar los ejemplos como programas generales.

El README de [Practica](Curso_2026_2027/Practica) es todavía mínimo; utiliza las páginas de cada ejercicio.

<a id="secuencial"></a>

## Programación secuencial y prácticas anteriores

### Ejercicios propios

En [EJERCICIOS MIOS](SECUENCIAL/EJERCICIOS%20MIOS) hay ejercicios para generar una matriz NxM, calcular una media global, trabajar con elementos menores que la media y crear bordes de matrices.

La subcarpeta [Vecinos](SECUENCIAL/EJERCICIOS%20MIOS/Vecinos) contiene modificar_vecinos.c y el esquema VECINOS.png para estudiar modificaciones de posiciones vecinas.

### Prácticas de 2025–2026

| Carpeta | Contenido |
|---|---|
| [SECUENCIAL/EJERCICIOS](SECUENCIAL/PracticasEnSecuencial2025-26/SECUENCIAL/EJERCICIOS) | P0-1.c a P0-8.c, generador de ficheros y esquema de P0-8. Ejercicios de lectura y tratamiento de vectores, matrices e imágenes; P0-8 incluye cálculo de desviación típica. |
| [SECUENCIAL/ENUNCIADOS](SECUENCIAL/PracticasEnSecuencial2025-26/SECUENCIAL/ENUNCIADOS) | Enunciados de las ocho prácticas en imágenes PNG. |
| [SECUENCIAL/Archivos](SECUENCIAL/PracticasEnSecuencial2025-26/SECUENCIAL/Archivos) | Matriz binaria de entrada de 150×150. |
| [SECUENCIAL_MPI](SECUENCIAL/PracticasEnSecuencial2025-26/SECUENCIAL_MPI) | Enunciados en PNG de las versiones secuenciales vinculadas a las prácticas MPI. |

El código y los enunciados son material de estudio; algunas versiones adaptan el problema original, como indica el comentario de P0-1.c.

<a id="mpi"></a>

## MPI

MPI trabaja con procesos y memoria distribuida. La carpeta [MPI/Ejercicios_clase](MPI/Ejercicios_clase) contiene esta progresión:

| Ejemplo | Concepto |
|---|---|
| [ej0.c](MPI/Ejercicios_clase/ej0.c) | Inicializar y finalizar MPI. |
| [ej1.c](MPI/Ejercicios_clase/ej1.c) | Consultar rango y número de procesos. |
| ej2.c, ej3.c, ej4.c, ej4b.c, ej5.c y ej5b.c | Variantes de comunicación punto a punto con Send y Recv. |
| ej5c.c y ej5d.c | Difusión con Bcast. |
| ej6.c y ej7.c | Reduce y Allreduce. |
| [ej8.c](MPI/Ejercicios_clase/ej8.c) | Prefijos con Scan. |
| ej9.c y ej10.c | Gather y Gatherv. |
| ej11.c y ej12.c | Scatter y Scatterv. |
| [ej13.c](MPI/Ejercicios_clase/ej13.c) | Reduce_scatter. |
| ej14.c y ej15.c | Allgather y Allgatherv: todos reciben los bloques reunidos. |
| ej16.c y ej17.c | Alltoall y Alltoallv: intercambio de bloques entre todos. |
| [ej_grupos.c](MPI/Ejercicios_clase/ej_grupos.c) | Grupos, creación de comunicadores y Allreduce en un subgrupo. Incluye llamadas a Barrier comentadas. |
| [ejemplo_prod_int_c.c](MPI/Ejercicios_clase/ejemplo_prod_int_c.c) | Reparto de datos y combinación de resultados para un producto interno usando mensajes. |

La tabla describe las llamadas que aparecen en el material; no implica que todos los archivos estén validados para cualquier número de procesos.

### Colectivas con diagramas y equivalentes punto a punto

Las páginas de [Funciones colectivas](Curso_2026_2027/Apuntes%20Clase/MPI/Funciones%20colectivas/README.md) cubren Bcast, Reduce, Allreduce, Gather, Gatherv, Scatter, Scatterv, Reduce_scatter, Barrier y Scan.

En [Send_Recv](Curso_2026_2027/Apuntes%20Clase/MPI/Funciones%20colectivas/Send_Recv/README.md) cada página compara el bloque de la colectiva original con un programa completo que usa MPI_Send y MPI_Recv, explica las fases y muestra el resultado esperado. Los ejemplos usan exactamente cuatro procesos, enteros y suma para las reducciones.

Allgather, Allgatherv, Alltoall y Alltoallv aparecen en los ejercicios antiguos, pero todavía no tienen páginas equivalentes en estas dos carpetas.

### Prácticas y datos MPI

[MPI/Practica](MPI/Practica) contiene enunciados en PNG de las prácticas 1 y 2. En [archivos_asignatura](MPI/Practica/archivos_asignatura) están la matriz150x150.bin, imágenes Lena RAW de 512×512 y 4096×4096, y variantes de 512×512 llamadas media y mediaponderada.

La subcarpeta [solucion](MPI/Practica/archivos_asignatura/solucion) contiene resSec1.txt y resSec2.txt como resultados de referencia. Esta carpeta aporta enunciados y datos; no contiene una implementación C completa de esas prácticas paralelas.

### Scripts MPI

- [compile.sh](MPI/Ejercicios_clase/compile.sh) compila ej0.c con mpicc y crea el ejecutable ej.
- [run.sh](MPI/Ejercicios_clase/run.sh) ejecuta ej con cuatro procesos.
- [lanzar_slurm_mpi.sh](MPI/Ejercicios_clase/lanzar_slurm_mpi.sh) contiene una plantilla para un clúster con Slurm. Sus parámetros de partición, recursos y lanzamiento dependen de aquel entorno; no es el comando de ejecución local en WSL.

<a id="openmp"></a>

## OpenMP

OpenMP trabaja con hilos y memoria compartida. En [OpenMP/Ejercicios_clase](OpenMP/Ejercicios_clase) se encuentran:

| Ejemplos | Conceptos |
|---|---|
| ejemplo1.c a ejemplo4.c | Regiones parallel y número de hilos. |
| ejemplo5.c a ejemplo8.c | Reparto de bucles con for y planificación static, dynamic y guided. |
| ejemplo9.c | sections y section. |
| ejemplo10.c y ejemplo11.c | single, con y sin nowait. |
| ejemplo12.c y ejemplo13.c | master. |
| ejemplo14.c y ejemplo15.c | ordered. |
| ejemplo16.c a ejemplo19.c | critical, barrier, atomic y flush. |
| ejemplo20.c a ejemplo24.c | Ámbito de variables y cláusulas single, default, firstprivate, threadprivate y lastprivate. |
| ejemplo25.c | threadprivate y copyin. |
| ejemplo26.c | Reducción de un producto interno. |
| ejemplo27.c | single con copyprivate. |
| ejemplo_copyin.c | Ejemplo con threadprivate y una cláusula copyin comentada para comparar su comportamiento. |
| ejemplo_pi_openmp.c | Variantes de cálculo de producto interno, reparto de trabajo y sincronización. Aquí “pi” se refiere al producto interno. |

[compilar.sh](OpenMP/Ejercicios_clase/compilar.sh) compila ejemplo1.c con gcc -fopenmp y genera ejemplo; [run.sh](OpenMP/Ejercicios_clase/run.sh) lo ejecuta.

En [OpenMP/Practica](OpenMP/Practica) hay enunciados PNG de las prácticas 1 y 2. [archivos_asignatura](OpenMP/Practica/archivos_asignatura) aporta matriz150x150.bin y el resultado resSec1.txt en Solucion. Como en MPI/Practica, se trata de material para desarrollar las prácticas, no de soluciones paralelas completas en C.

<a id="youtube"></a>

## Material de YouTube

La carpeta [YouTube](YouTube) agrupa archivos que acompañan los episodios. No contiene los vídeos ni un índice de enlaces al canal.

| Episodio | Contenido |
|---|---|
| [EP1](YouTube/EP1) | Programa básico de lectura de una matriz binaria, argumentos y memoria dinámica, con varios ficheros de prueba. |
| [EP2](YouTube/EP2) | P0-3.c, enunciado en PNG y matrices/vectores binarios. |
| [EP3](YouTube/EP3) | P0-4.c, ejemplo de aritmética de punteros, enunciado, datos y presentación PPTX sobre punteros. |
| [EP4](YouTube/EP4) | P0-5.c, enunciado, matriz de entrada y ejecutable. |
| [EP5](YouTube/EP5) | Presentación PARALELA UD 1.odp. |
| [EP6](YouTube/EP6) | Presentación Paralela UD2.odp. |
| [EP7](YouTube/EP7) | Cuatro ejemplos introductorios MPI: inicio/finalización, rango, tamaño y mensajes punto a punto. |
| [EP8](YouTube/EP8) | P0_T1: fichero binario con dos vectores, preguntas y respuestas, versiones con índices y punteros, y explicación índices vs. punteros. |
| [EP9](YouTube/EP9) | P0_T2: enunciado, memoria dinámica con una o dos reservas y fichero de entrada. |
| [EP10](YouTube/EP10) | Enunciado de P0_T3 sobre aritmética y tipos de punteros, con datos de entrada. No hay un .c en esta carpeta. |

EP8 y EP9 comparten ejercicios con las prácticas actuales; consulta ambas ubicaciones si buscas explicaciones complementarias.

<a id="datos-y-herramientas"></a>

## Datos, formatos y herramientas

| Archivos | Uso |
|---|---|
| .c | Código fuente; cada ejemplo se compila por separado. |
| .md | Apuntes, enunciados, índices y explicaciones. |
| .pdf y .png | Notas, enunciados y esquemas. |
| .docx, .pptx y .odp | Guía de estudio y presentaciones. |
| .bin y .raw | Datos binarios; hay que conocer sus dimensiones y el tipo de sus elementos. No todos los .bin almacenan el mismo tipo. |
| resSec*.txt | Resultados secuenciales de referencia en las carpetas de prácticas. |
| .exe | Ejecutables de Windows presentes en algunos ejercicios; utiliza el fuente para recompilar en Linux. |

En la raíz también están [matriz.bin](matriz.bin), [matriz.txt](matriz.txt) y prueba.exe, como archivos sueltos de datos/ejecución. Para escoger una entrada, sigue el enunciado y los argumentos del programa concreto; no hay una orden universal que sirva para todos.

En [.vscode](.vscode):

- [tasks.json](.vscode/tasks.json) define una tarea local con C:/MinGW/bin/gcc.exe para compilar el archivo activo. No compila MPI mediante WSL y su ruta puede necesitar cambios en otro PC.
- [Configurar-MPI-IntelliSense.ps1](.vscode/Configurar-MPI-IntelliSense.ps1) crea la configuración y cabecera auxiliares descritas abajo.
- c_cpp_properties.json configura IntelliSense; mpi-intellisense/mpi.h declara la parte de MPI utilizada en los ejemplos nuevos.

[.gitattributes](.gitattributes) configura la detección de archivos de texto y normalización de finales de línea para Git.

<a id="compilar"></a>

## Compilar y ejecutar

En Linux o Ubuntu/WSL necesitas un compilador C; para MPI, sus herramientas y cabeceras de desarrollo; para OpenMP, un compilador con soporte OpenMP. Ejecuta cada comando desde la carpeta del ejemplo y comprueba sus argumentos de entrada.

Ejemplo MPI independiente, con cuatro procesos:

```bash
cd "Curso_2026_2027/Apuntes Clase/MPI/Funciones colectivas/Send_Recv"
mpicc -std=c99 08_Reduce_scatter.c -o 08_Reduce_scatter
mpiexec -n 4 ./08_Reduce_scatter
```

Ejemplo OpenMP, desde la raíz:

```bash
cd OpenMP/Ejercicios_clase
gcc -fopenmp ejemplo1.c -o ejemplo1
./ejemplo1
```

Para código secuencial, usa gcc con el archivo concreto; añade -lm si requiere funciones de la biblioteca matemática. Los programas que leen ficheros o reciben dimensiones necesitan sus argumentos propios. Los scripts existentes compilan ejemplos específicos, no todos los archivos del repositorio.

<a id="vscode-wsl"></a>

## Configurar VS Code en Windows cuando MPI está en WSL

Si compilas en Ubuntu mediante WSL, pero abres el proyecto en una ventana local de VS Code para Windows, IntelliSense puede mostrar `cannot open source file "mpi.h"` (error 1696). El editor y el compilador están trabajando en entornos diferentes.

Puedes usar el script [Configurar-MPI-IntelliSense.ps1](.vscode/Configurar-MPI-IntelliSense.ps1) para que el editor local encuentre las declaraciones MPI de los ejemplos, manteniendo activos los demás diagnósticos de C/C++.

### Pasos en este PC o en otro ordenador

1. Descarga o clona el repositorio completo, incluida la carpeta `.vscode`.
2. Instala VS Code y la extensión **C/C++ de Microsoft**.
3. Abre la **carpeta raíz del repositorio** en VS Code, donde está este README. Las rutas de configuración se calculan desde esa carpeta.
4. En una terminal **PowerShell de Windows**, situada en la raíz del repositorio, ejecuta:

   ```powershell
   powershell.exe -NoProfile -ExecutionPolicy Bypass -File ".\.vscode\Configurar-MPI-IntelliSense.ps1"
   ```

   El permiso de ejecución se aplica solo a ese proceso; no cambia la política de PowerShell permanentemente.

5. Pulsa `Ctrl + Shift + P`, ejecuta **C/C++: Select a Configuration** y selecciona **Win32**.
6. Si sigue apareciendo el aviso, ejecuta **C/C++: Reset IntelliSense Database**. Si hace falta, usa **Developer: Reload Window**.

### Ajustar el compilador en otro PC

El script genera `.vscode/c_cpp_properties.json` con el compilador local usado en este ordenador: `C:/MinGW/bin/gcc.exe`.

Si tu instalación está en otra ruta, ejecuta **C/C++: Edit Configurations (UI)** y cambia **Compiler path** por la ruta de tu compilador de Windows. Ajusta también **IntelliSense mode** a su arquitectura: por ejemplo, `windows-gcc-x64` para GCC de 64 bits. Este compilador configura el análisis del editor; los ejemplos MPI se siguen compilando con `mpicc` dentro de Ubuntu.

Si no tienes un compilador de C en Windows, puedes abrir directamente el proyecto con **WSL: Reopen Folder in WSL** y usar C/C++ instalado en WSL con las cabeceras MPI reales.

### Qué crea el script y qué limitaciones tiene

- `.vscode/mpi-intellisense/mpi.h`: declaraciones auxiliares para los ejemplos de funciones colectivas y sus mensajes Send/Recv.
- `.vscode/c_cpp_properties.json`: configuraciones **Win32** y **Linux**. La configuración Win32 añade la carpeta auxiliar al `includePath`.

La cabecera auxiliar **no es una instalación de MPI ni contiene su implementación**. Solo permite analizar los ejemplos locales; no cubre toda la API MPI ni garantiza que una llamada sea válida para tu instalación real. Los demás diagnósticos permanecen activos dentro de lo que IntelliSense pueda analizar con estas declaraciones.

No añadas esa carpeta auxiliar a las opciones de compilación ni copies su cabecera a Ubuntu. Tiene una protección para rechazar su uso fuera de IntelliSense de Windows. Los archivos `.c` conservan su `#include <mpi.h>` original.

El script se detiene si ya existe `c_cpp_properties.json` para no sobrescribir tu configuración. Si ese archivo viene incluido al descargar el repositorio, basta con revisar las rutas y seleccionar **Win32**; no necesitas ejecutar el script otra vez.

### Compilar y ejecutar de verdad en Ubuntu / WSL

Desde la carpeta del programa, en una terminal de Ubuntu con MPI instalado:

```bash
mpicc -std=c99 08_Reduce_scatter.c -o 08_Reduce_scatter
mpiexec -n 4 ./08_Reduce_scatter
```

La comprobación definitiva del código MPI la realiza ese compilador con las cabeceras y bibliotecas reales de Ubuntu. La configuración local de IntelliSense no cambia estos comandos.

### Deshacer esta configuración local

En `.vscode/c_cpp_properties.json`, elimina de la configuración Win32 la ruta `mpi-intellisense` del `includePath`. Después puedes borrar la carpeta `.vscode/mpi-intellisense` y reiniciar IntelliSense. Si tenías una configuración anterior, conserva sus demás ajustes.

<a id="consejos"></a>

## Consejos de estudio y exámenes

Domina ficheros, memoria, punteros, vectores y matrices antes de empezar a paralelizar. Practica las trazas a mano, compara los resultados con una versión secuencial y entiende el reparto del trabajo antes de elegir las comunicaciones o directivas.

El texto original del repositorio se conserva debajo. Incluye experiencias del autor, recomendaciones y referencias a evaluaciones de años anteriores. Para los criterios y fechas del curso actual, consulta el enunciado y las indicaciones docentes correspondientes.

<details>
<summary>Leer las recomendaciones originales sobre prácticas y exámenes</summary>

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

</details>

<a id="colaboradores"></a>

## Colaboradores

- abopder
- lorenaalmoguera

El repositorio nació para ayudar al estudiantado de la UMH a preparar Computación Paralela. Se agradecen correcciones de explicaciones, enlaces, ejemplos y resultados, indicando el archivo y el curso al que corresponden.
## Revisiones de los apuntes

Las correcciones actuales están en Markdown: [índice de estudio](Curso_2026_2027/Apuntes/README.md), [revisión del 11 de septiembre](Curso_2026_2027/Apuntes%20Clase/UD1/11_septiembre_revisado.md), [revisión del 29 de septiembre](Curso_2026_2027/Apuntes%20Clase/MPI/29_septiembre_revisado.md) y [clase del 6 de octubre](Curso_2026_2027/Apuntes%20Clase/MPI/06_octubre.md). Los PDF originales se conservan y la guía Word corresponde a la edición anterior. El [informe para consultar al profesor](Curso_2026_2027/Apuntes/Revision_conceptual_para_profesor.md) distingue errores y simplificaciones.

