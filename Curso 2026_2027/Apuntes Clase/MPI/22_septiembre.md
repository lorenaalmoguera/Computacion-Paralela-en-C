# Recapitulamos

Recordamos que una vez que tengo mi código secuencial funcional, empiezo a implementar el código paralelo.

El código deberá funcionar para cualquier tamaño de problema y cualquier número de hilos.

Ese código se podrá ejecutar en cualquier computador conectado a una red de interconexión que estarán conectados por una red.

Vamos a utilizar MPI para explotar esa arquitectura HPC.

Sabemos que es MPI que permite la comunicación de estas arquitecturas HPC y que tiene una libreria que lo que forma es una capa exterior a ese sistema en el que quiero comunicar. 

Como el diseño paralelo y mi codificación en lo que respecta a las reservas de memoria afecta a los patrones de comunicación, se obtiene todo del primer paso que es el diseño paralelo.

Código debe funcionar para cualquier tamaño de problema y cualquier número de procesos.
El cálculo de reparto debenderá de cuantos procesos hay en ese momento.

Compilación:
* Automatiza el lanzamiento paralelo
* Pone en marcha una serie de memoria e identificadores de esa ejecución paralela que acaba de ponerse en marcha.

## `MPI_Init(&argc, &argv);`

Lo más pronto posible y se pasan en referencia las variables

## `MPI_Finalize();`

Lo más tarde posible.

# Sesión 22 Septiembre

## Cuantos procesos se están ejecutando.
Con esta función sabremos cuantos procesos se están ejecutando en esta ejecución.
```c
MPI_Comm_size(MPI_COMM_WORLD, &nproces);
```
* `MPI_COMM_WORLD` el comunicador 
* `nproces` variable que yo paso por referencia para que me devuelva un valor, que será el número de procesos que se están ejecutando en este momento.

Cuando yo inicializo una ejecución, lo inicializo con 100 procesos. En ese momento, se crea un grupo de procesos donde están todos los procesos (0-99). Este grupo de procesos tiene un comunicador que es `MPI_COMM_WORLD`.

Es decir, al crear la ejecución, a esos procesos se les asocia un comunicador que es `MPI_COMM_WORLD`, y este no se puede tocar.

Existe la posibilidad de tener muchos grupos de procesos, comunicadores, definidos.

Cuando se lanza el mpi run lo que se hace es se crea el comunicador por defecto, con los procesos ejecutados.

Esta función solo tiene que devolvernos el valor de la cantidad de procesos, pero tendremos que usar el valor por referencia, ya que si hacemos

```c
int ierr;
int ierr = MPI_Comm_size(MPI_COMM_WORLD, &nproces);
```

el `int ierr` devolverá el código de error.

En el momento en el que sabemos la cantidad de procesos que se están ejecutando podremos proceder a relaizar el reparto de trabajo.

nproces devuelve un valor, por eso lo llamamos con el ``&`` porque buscamos obtener el valor que se esconde en esa dirección de memoria.

> `nproces` almacenará el número de procesos que se están ejecutando. Como `MPI_Comm_size` necesita escribir ese valor en la variable, le pasamos su dirección de memoria mediante `&nproces`.


## que proceso soy yo de todos esos procesos

```c
int ierr;
int myrank = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
```

devolverá el identificador del proceso actual. Es decir si tenemos 4 ejecuciones, nos devolverá 0 1 2 y 3, 1 numero en cada ejecución. con esto podremos empezar a paralelizar, ya que sabremos cuantos somos y que proceso es quien.

## Que es MPI_COMM_WORLD

Un comunicador predefinido para el grupo de procesos global, la herramienta que se usa para que se comuniquen esos procesos.

El comunicador referenciará el grupo para poder comunicarse.

La creación de procesos en MPI 1 no es dinámica. Si lanzo mi ejecución mpirun hay 20 procesos desde el principio hasta el final. Entonces si tienes un grupo con 20 procesos y otros con 10, tendrás procesos que pertenecen a 2 grupos, y el número de procesos no cambiará, este será estático.

Como el único grupo que conocemos es MPI_COMM_WORLD, en la gran mayoría de funciones lo utilizarémos.

```c

int main(int argc, char* argv[]){
    int nproces, myrank, err;
    err = MPI_Init(&argc, &argv);
    err = MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    printf("Soy el proceso %d de %d procesos.\n", myrank, nproces);
    err = MPI_Finalize();
}
```

Cual es la salida de este programa cuando lo ejecute?
* 4 líneas de texto
```text
Soy el proceso 0 de 4 procesos
Soy el proceso 3 de 4 procesos
Soy el proceso 1 de 4 procesos
Soy el proceso 2 de 4 procesos
```

La salida será una línea de texto
La línea de texto cinluye el valor actual de dos variables (myrank y nproces)
El valor de las variables myrank y nproces se ha actualizado en las llamadas.


Si recordamos que tenemos 4 computadores conectados por una red de interconexión, y tenemos un programa que lo compilamos y lo ejecutamos en los 4... tendremos 4 variables nproces y myrank en total. 1 en cada uno de las memorias locales de los computadores.

Tendremos que monitorizar 4 ejecuciones distintas del mismo código proque queremos que hagan cosas distintas.

En algún momento llegarán a la función de ``MPI_Comm_size()``. Habrá uno que obtendrá el valor 3, otro 2, otro 1 y otro 0. No sabremos quien lo tiene, por eso tendremos que preguntarlo con la función.

Cuando evaluamos la traza evaluamos la traza en los 4 procesos. En el ejemplo que estamos hablando, la traza será la siguiente:
```text
Soy el proceso 0 de 4 procesos
Soy el proceso 3 de 4 procesos
Soy el proceso 1 de 4 procesos
Soy el proceso 2 de 4 procesos
```

Cada uno lanza y ejecuta su instrucción de printf.

Para depurar esto va a ser un infierno si tenemos que conectarnos en ordenadores distintos para ver cual es la salida. En el sistema que vamos a trabajar en prácitcas, sobre todo los gestores de colas, lo que hace es recoger todas las salidas, las junta, y te los manda en un fichero.

No tengo posibilidad alguna de saber quien ha producido cada una de esas señales. Cuando depure y ponga printf para depurar en ejecución, no tiene ningún sentido. Siempre pondré el identificador de ``myrank`` para saber que proceso ha ejecutado esa línea.

### Salida de una ejecución simultánea

```text
Ejecución 1
Soy el proceso 2 de 4 procesos
Soy el proceso 0 de 4 procesos
Soy el proceso 3 de 4 procesos
Soy el proceso 1 de 4 procesos

Ejecución 2
Soy el proceso 1 de 4 procesos
Soy el proceso 3 de 4 procesos
Soy el proceso 0 de 4 procesos
Soy el proceso 2 de 4 procesos

Ejecución 3
Soy el proceso 3 de 4 procesos
Soy el proceso 2 de 4 procesos
Soy el proceso 1 de 4 procesos
Soy el proceso 0 de 4 procesos

Ejecución 4
Soy el proceso 0 de 4 procesos
Soy el proceso 2 de 4 procesos
Soy el proceso 1 de 4 procesos
Soy el proceso 3 de 4 procesos

Ejecución 5
Soy el proceso 1 de 4 procesos
Soy el proceso 0 de 4 procesos
Soy el proceso 2 de 4 procesos
Soy el proceso 3 de 4 procesos

Ejecución 6
Soy el proceso 3 de 4 procesos
Soy el proceso 1 de 4 procesos
Soy el proceso 2 de 4 procesos
Soy el proceso 0 de 4 procesos
```

El orden no tiene porque ser siempre el mismo.

## Identificación de procesos


```c

int main(int argc, char* argv[]){
    int nproces, myrank, err, i = 0;
    err = MPI_Init(&argc, &argv);
    err = MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    i++;
    printf("Soy el proceso %d de %d procesos.\n. Mi variable i %d vale", myrank, nproces);
    err = MPI_Finalize();
}
```

`i` valdrá 1 en todos los procesos, ya que la variable existirá en todas las ejecuciones como 0 incialmente y se incrementará a 1.

El orden de salida será independiente una vez más.

Lo que haga un proceso sobre una variable no afecta a las variables de los otros. Cuando queramos que afecte tendremos que hacer una comunicación.

## Comunicaciones en MPI
Antes de enviar información, lo que haremos es reservar la memoria que necesitamos. En algun momento se les tendrá que informar de cuanta memoria tienen que reservar, ya que posteriormente se les enviarán datos.

### ENVÍO

Yo envío un mensaje, un mensaje es una zona contigua de memoria que contiene un conjunto de datos que son los que quiero envíar.

Especificamos dichos datos especificando el inicio de la zona de memoria a enviar y el tamaño de la misma en bytes. El inicio de memoria se especifica con un puntero, es decir con la dirección de memoria del primer dato a enviar.


#### MPI_Send
```c
err = MPI_Send(DatosEnv, NumDatos, TipoDatos, Destino, tag, comunicador);
```

Tengo que decir, 
* DatosEnv - donde empiezan los bytes que quiero enviar -> será una dirección de memoria, es decir, puntero
* Número de Datos -> Cuantos bytes quiero enviar
* TipoDatos -> Tipo de dato a enviar
* Destino -> de un proceso a otro proceso, es decir, el rango del proceso que tendrá que recibir, será un número entero. (0-(nproces-1))
* Tag -> Identificador del envío. Recordamos que podemos hacer muchos envíos.
* Comunicador -> el que contiene el grupo de procesos


Preguntas:

* Qué envío -> un mensaje               Qué es un mensaje -> una zona contigua de memoria que contiene un conjunto de datos que son los que quiero envíar.
* Quién envía -> un proceso             Cómo se identifica -> el que lo ejcuta
* Quién recibe -> otro proceso          Cómo se identifica -> con un entero


```c
MPI_Send(
    void* DatosEnv,
    int NumDatos,
    MPI_Datatype TipoDatos,
    int Destino,
    int Tag,
    MPI_Comm comunicador);
```
MPI_Datatype las opciones estarán en el `include <mpi.h>`:

> MPI_CHAR, MPI_SHORT, MPI_INT, MPI_INTEGER, MPI_INT4, MPI_LONG, etc...

Usaremos:

> MPI_INTEGER, MPI_DOUBLE

Si ponemos como destino un numero de proceso que no existe, es decir en 5 ejecuciones ponemos un 15 el programa de por si no petará pero estará mal. Si hay suerte petará pero no tiene porque.

* Mensaje: conjunto de datos almacenados en una zona contigua de memoria
* Inicio: DatosEnv
* Tamaño: (NumDatos, TipoDatos)

* Destino: identificador (número entero) del proceso que va a recibir datos. Obligatoriamente los tiene que recibir una función de recibir.

* Tag: es una etiqueta (número entero) que debe ser el mismo en la función de envío que en la función de recepción.

* Comunicador: MPI_COMM_WORLD

> Si mandamos mas datos de los que tenemos estaremos mandando cosas que se encuentran en la zona de memoria que no nos pertenecen. Es decir, estaremos enviando una información que no deberíamos de enviar. (e.g enviar 300 cuando nuestro mensaje es de tamaño 200). Por lo tanto, tendremos un problema de memoria `segmentation fault` y un problema conceptual

### RECEPCIÓN

#### MPI_RECV

```c
err = MPI_Recv(DatosRec, NumDatos, TipoDatos, Origen, tag, Comunicador, status)
```

Un proceso nunca se envirá datos a si mismo ya que no haces el código portable, no todas las distribuciones lo permiten.
Los procesos de comunicación son cooperativos intervienen en emisor y el receptor y habrá que identificar esta comunicación con un tag, por lo que tendrá que ser el mismo en la recepción y el envío.

Status nos dará la información del envío ya que la recepción no retornará nada (por eso es &status con &) hasta que se reciba.

Preguntas

* Qué recibo -> un mensaje
* Dónde lo almaceno -> en memoria, nos tenemos que asegurar de tener el espacio reservado para almacenar el mensaje de ese tamaño y que en esa zona no hubiese nada que necesitabamos ya que en ese caso se sobreescribirá
* De qué tamaño -> se obtiene por (NumDatos, TipoDatos)
* Quién envía -> el proceso con rango origen
* Quién recibe? el que ejecuta la función de recepción

```c
MPI_Recv(
    void* DatosRec,
    int NumDatos,
    MPI_Datatype TipoDatos,
    int Origen,
    int tag,
    MPI_Comm comunicador,
    MPI_Status* status);
```

Si status nos devuelve la infromación es porque en algunos casos necesitaremos consultar la información.

Que pasa si el mensaje que yo espero recibir se envió hace tiempo?

No pasará nada, ese mensaje estará disponible cunado yo llame a la función y ya está.
No hay ningún problema ya que MPI se encarga de sincronizar estos procesos.

Tienen que estar emisor y receptor ejeuctando sus funciones mpi_send y mpi_recv a la vez? no porque se encarga de sincronizar `<mpi.h>`. Como se hayan producido en el tiempo esas funciones no nos importa. No tenemos que hacer funcion de errores. Si algo no funciona es porque hemos programado mal.

### Entender MPI recepción y envío

si yo tengo un envio y recepcion de un escalar de un proceso dado 0 a otro 3, yo lo unico que quiero es que el escalar de una variable data1 a una variable que se puede llamar data1, data2 (el nombre es irrelevante), vamos a llamarlo data2.


No hay relación en el mapa de memoria entre un proceso y otro. Cuando vemos punteros, cada puntero irá a su zona de memoria.
Comunicamos datos, y para comunicar datos usamos punteros.


```c

int main(int argc, char* argv[]){
    int nproces, myrank, err;
    double data1 = 0, data2 = 0;
    MPI_Status status;

    err = MPI_Init(&argc, &argv);
    err = MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    if (myrank == 0){
        data1 = 47645,3;
        MPI_Send(&data1, 1, MPI_DOUBLE, 3, 99, MPI_COMM_WORLD); // que enviamos, cuantos datos, que tipo de datos, a quien, que tag, que comunicador
    }else{
        if(myrank == 3){
            MPI_Recv(&data2, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &status); // donde recibimos, cuanto recibimos, que tipo de dato, de quien, que tag, que comunicador, donde guardamos el status.
        }
    }

    err = MPI_Finalize();
}
```

En la traza observamos que para 4 ejecuciones, los unicos que hacen algo son los procesos 0 y 3. El resto de procesos siguen haciendo la ejecución pero no entrarán en el if.

El código funcionará para cualquier número de procesos mayor a 3. 

Si pensais que las comunicaciones se hacen justo cuando están en el send y recv podremos desarrollar los códigos.

La función de recepción es bloqueante. Puede pasar 2 cosas:
* que ya le hayan enviado y tiene la información en el buffer (recogerlo es pillarlo del buffer y ponerlo en la posicion de memoria especificado en la recepcion)
* que nunca le envian y se queda esperando

Que pasa si...



```c

int main(int argc, char* argv[]){
    int nproces, myrank, err;
    double data1 = 0, data2 = 0;
    MPI_Status status;

    err = MPI_Init(&argc, &argv);
    err = MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    if (myrank == 0){
        data1 = 47645,3;
        MPI_Send(&data1, 1, MPI_DOUBLE, 3, 99, MPI_COMM_WORLD); // que enviamos, cuantos datos, que tipo de datos, a quien, que tag, que comunicador
    }else{
        if(myrank == 3){
            MPI_Recv(&data2, 1, MPI_DOUBLE, 0, 88, MPI_COMM_WORLD, &status); // donde recibimos, cuanto recibimos, que tipo de dato, de quien, que tag, que comunicador, donde guardamos el status.
        }
    }

    err = MPI_Finalize();
}
```

El proceso 3 nunca saldrá de la ejecución porque se esperará a recibir los datos que nunca recibirá. Entonces lo que vemos es que nadie ha acabado la ejecución porque `MPI_Finalize()` es una barrera de sincronización que se espera a que lleguen todos para termiinar.

Que pasa si

```c

int main(int argc, char* argv[]){
    int nproces, myrank, err;
    double data1 = 0, data2 = 0;
    MPI_Status status;

    err = MPI_Init(&argc, &argv);
    err = MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    if (myrank == 0){
        data1 = 47645,3;
        MPI_Send(&data1, 1, MPI_DOUBLE, 2, 99, MPI_COMM_WORLD); // que enviamos, cuantos datos, que tipo de datos, a quien, que tag, que comunicador
    }else{
        if(myrank == 3){
            MPI_Recv(&data2, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &status); // donde recibimos, cuanto recibimos, que tipo de dato, de quien, que tag, que comunicador, donde guardamos el status.
        }
    }

    err = MPI_Finalize();
}
```

Ocurre exactamente lo mismo que en el caso anterior.
Pese a que las etiquetas parece son el mismo mensaje, el emisor y receptor no coinciden.


Que pasa si

```c

int main(int argc, char* argv[]){
    int nproces, myrank, err;
    double data1 = 0, data2 = 0;
    MPI_Status status;

    err = MPI_Init(&argc, &argv);
    err = MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    if (myrank == 0){
        data1 = 47645,3;
        MPI_Send(&data1, 1, MPI_DOUBLE, 3, 99, MPI_COMM_WORLD); // que enviamos, cuantos datos, que tipo de datos, a quien, que tag, que comunicador
    }else{
        if(myrank == 3){
            MPI_Recv(&data2, 1, MPI_DOUBLE, 1, 99, MPI_COMM_WORLD, &status); // donde recibimos, cuanto recibimos, que tipo de dato, de quien, que tag, que comunicador, donde guardamos el status.
        }
    }

    err = MPI_Finalize();
}
```

Ocurre exactamente lo mismo otra vez.

Que pasa si

```c

int main(int argc, char* argv[]){
    int nproces, myrank, err;
    double data1 = 0, data2 = 0;
    MPI_Status status;

    err = MPI_Init(&argc, &argv);
    err = MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    if (myrank == 0){
        data1 = 47645,3;
        MPI_Send(&data1, 1, MPI_DOUBLE, 3, 99, MPI_COMM_WORLD); // que enviamos, cuantos datos, que tipo de datos, a quien, que tag, que comunicador
    }else{
        if(myrank == 100){
            MPI_Recv(&data2, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &status); // donde recibimos, cuanto recibimos, que tipo de dato, de quien, que tag, que comunicador, donde guardamos el status.
        }
    }

    err = MPI_Finalize();
}
```

Habrá una función de envío que no se ha finalizado. MPI_Finalize() sincronizará y terminará, pero MPI al finalizar nos indicará que hay un proceso que tiene información en su buffer que no se ha recibido y lo vaciará.


#### Lo mismo pero con vectores ahora

ejemplo:
El proceso 1 le envia a todos los demas procesos una determinada información, es decir un boradcast
```c
int main(int argc, char* argv[]){
    int nproces, myrank, err;
    int i_vector1[16];
    MPI_Status status;

    err = MPI_Init(&argc, &argv);
    err = MPI_Comm_size(MPI_COMM_WORLD, &nprocess);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    if (myrank == 1){
        for(int i = 0 ; i < nproces ; i++){
            if(i!= 1) MPI_Send(i_vector1, 16, MPI_INTEGER, i, 99, MPI_COMM_WORLD); // computacionalmente la gente no suele hacer el if fuera, pero en verdad sería hacer un send al 0 y luego un for a partir del 2 a i<nproces. en las prácticas mejor hacerlo así
        }
       
    }else{
        MPI_Recv(&i_vector1[0], 16, MPI_INTEGER, 1, 99, MPI_COMM_WORLD, &status);
    }

    err = MPI_Finalize();
}
```

> i_vector1 y &i_vector1[0] desde un punto de vista algorítmico es la posición inical del vector.


* las sentencias condicionales suelen indicar que una zona de código es ejecutada por 1 proceso u otro
* el número de envíos y recepciones depende de la ejecución paralela (variable nproces). la repetición de recepciones no la vemos en el código porque está en un else, sin embargo, tenemos que tener encuenta la cantidad de ejecuciones que están ocurriendo.