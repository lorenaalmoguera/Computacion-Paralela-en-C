# Funciones colectivas

## MPI_Bcast

```mermaid
flowchart LR
 A["P0 · raíz<br/>dimensión = 512"] --> B["MPI_Bcast · count=1 · MPI_INT · raíz=0"]
 B --> C["P0 · dimensión = 512"]
 B --> D["P1 · recibe 512<br/>dimensión = 512"]
 B --> E["P2 · recibe 512<br/>dimensión = 512"]
```
## MPI_Reduce

```mermaid
flowchart LR
 A["P0 · V1[1,23,2]"] --> R["MPI_Reduce<br/>MPI_SUM · count=3 · raíz=0"]
 B["P1 · V2[15,7,8]"] --> R
 R --> C["P0 · resultado<br/>[1+15,23+7,2+8]<br/>[16,30,10]"]
```

## MPI_Allreduce

```mermaid
flowchart LR
 A["P0 · V1[1,23,2]"] --> R["MPI_Allreduce<br/>MPI_SUM · count=3"]
 B["P1 · V2[15,7,8]"] --> R
 R --> C["P0 · [16,30,10]"]
 R --> D["P1 · [16,30,10]"]
```

## MPI_Gather

```mermaid
flowchart LR
 A["P0 · V1[1,23,2]"] -->|"3 elementos · posición 0"| G["MPI_Gather · raíz=0"]
 B["P1 · V2[15,7,8]"] -->|"3 elementos · posición 3"| G
 G --> C["P0 · V[1,23,2,15,7,8]"]
```

## MPI_Gatherv

```mermaid
flowchart LR
 A["P0 · [1]"] --> G["MPI_Gatherv · raíz=0<br/>recvcounts=[1,2,3]<br/>displs=[0,1,3]"]
 B["P1 · [23,2]"] --> G
 C["P2 · [15,7,8]"] --> G
 G --> D["P0 · V[1,23,2,15,7,8]"]
```

## MPI_Scatter

```mermaid
flowchart LR
 A["P0 · raíz<br/>V[1,23,2,15,7,8]"] --> S["MPI_Scatter<br/>sendcount=3 · recvcount=3"]
 S --> B["P0 · V1[1,23,2]"]
 S --> C["P1 · V2[15,7,8]"]
```

## MPI_Scatterv

```mermaid
flowchart LR
 A["P0 · raíz<br/>V[1,23,2,15,7,8]"] --> S["MPI_Scatterv<br/>sendcounts=[1,2,3]<br/>displs=[0,1,3]"]
 S --> B["P0 · [1]"]
 S --> C["P1 · [23,2]"]
 S --> D["P2 · [15,7,8]"]
```

## MPI_Reduce_scatter

```mermaid
flowchart LR
 A["P0 · V[1,23,2,15,7,8]"] --> R["MPI_Reduce_scatter<br/>MPI_SUM<br/>recvcounts=[3,3]"]
 B["P1 · V[1,23,2,15,7,8]"] --> R
 R --> T["Σ por posición<br/>[2,46,4,30,14,16]"]
 T --> C["P0 · [2,46,4]"]
 T --> D["P1 · [30,14,16]"]
```

## MPI_Barrier

```mermaid
sequenceDiagram
 participant A as P0 · V[1,23,2,15,7,8]
 participant B as P1 · V[1,23,2,15,7,8]
 participant C as P2 · V[1,23,2,15,7,8]
 A->>A: MPI_Barrier · entra
 Note over A: Espera
 B->>B: MPI_Barrier · entra
 Note over A,B: Esperan
 C->>C: MPI_Barrier · entra
 Note over A,C: Todos han entrado · salidas habilitadas
 Note over A,C: V sin cambios · sin transferencia de datos
```

## MPI_Scan

```mermaid
flowchart LR
 A["P0 · V1[1,23,2]"] --> S["MPI_Scan<br/>MPI_SUM · count=3"]
 B["P1 · V2[15,7,8]"] --> S
 S --> C["P0 · prefijo 0<br/>[1,23,2]"]
 S --> D["P1 · prefijo 0..1<br/>[16,30,10]"]
```

## MPI_Allgather

```mermaid
flowchart LR
 A["P0 · V1[1,23,2]"] --> G["MPI_Allgather<br/>sendcount=3 · recvcount=3"]
 B["P1 · V2[15,7,8]"] --> G
 G --> C["P0 · V[1,23,2,15,7,8]"]
 G --> D["P1 · V[1,23,2,15,7,8]"]
```

## MPI_Allgatherv

```mermaid
flowchart LR
 A["P0 · [1]"] --> G["MPI_Allgatherv<br/>recvcounts=[1,2,3]<br/>displs=[0,1,3]"]
 B["P1 · [23,2]"] --> G
 C["P2 · [15,7,8]"] --> G
 G --> D["P0 · V[1,23,2,15,7,8]"]
 G --> E["P1 · V[1,23,2,15,7,8]"]
 G --> F["P2 · V[1,23,2,15,7,8]"]
```

## MPI_Alltoall

```mermaid
flowchart LR
 A["P0 · V[1,23,2,15,7,8]<br/>sendcount=3 · recvcount=3"]
 B["P1 · V[1,23,2,15,7,8]<br/>sendcount=3 · recvcount=3"]
 A -->|"V1[1,23,2] → P0"| C["P0 · [1,23,2,1,23,2]<br/>bloques por origen: P0 | P1"]
 A -->|"V2[15,7,8] → P1"| D["P1 · [15,7,8,15,7,8]<br/>bloques por origen: P0 | P1"]
 B -->|"V1[1,23,2] → P0"| C
 B -->|"V2[15,7,8] → P1"| D
```
## MPI_Alltoallv

```mermaid
flowchart LR
 A["P0 · V1[1,23,2]<br/>sendcounts=[1,2] · sdispls=[0,1]"]
 B["P1 · V2[15,7,8]<br/>sendcounts=[2,1] · sdispls=[0,2]"]
 A -->|"[1]"| C["P0 · [1,15,7]<br/>recvcounts=[1,2] · rdispls=[0,1]"]
 A -->|"[23,2]"| D["P1 · [23,2,8]<br/>recvcounts=[2,1] · rdispls=[0,2]"]
 B -->|"[15,7]"| C
 B -->|"[8]"| D
```

# Comunicación punto a punto

## MPI_Send

```mermaid
flowchart TD
 A["P0 · V[1,23,2,15,7,8]"] --> B["MPI_Send · destino=1 · tag=0 · count=6"]
 B --> C{"Protocolo MPI"}
 C --> D["Copia interna posible"]
 C --> E["Espera al MPI_Recv de P1"]
 D --> F["Retorno de Send<br/>P0 puede reutilizar V"]
 E --> F
 D --> G["P1 · MPI_Recv<br/>V[1,23,2,15,7,8]"]
 E --> G
```

## MPI_Recv

```mermaid
sequenceDiagram
 participant A as P0 · emisor
 participant B as P1 · receptor
 B->>B: MPI_Recv · origen=0 · tag=0 · capacidad=6
 Note over B: Bloqueado hasta completar recepción
 A->>B: V[1,23,2,15,7,8] · tag=0
 B->>B: Retorno · V[1,23,2,15,7,8] disponible
```

## MPI_Ssend

```mermaid
sequenceDiagram
 participant A as P0 · V[1,23,2,15,7,8]
 participant B as P1
 A->>A: MPI_Ssend · destino=1 · count=6
 Note over A: Bloqueado
 B->>B: MPI_Recv correspondiente comienza
 A->>B: V[1,23,2,15,7,8]
 Note over A,B: Recepción iniciada + buffer emisor reutilizable
 A->>A: Ssend puede retornar
 B->>B: Recv retorna con V completo
```

## MPI_Bsend · MPI_Buffer_attach · MPI_Buffer_detach

```mermaid
sequenceDiagram
 participant A as P0 · V[1,23,2,15,7,8]
 participant M as Buffer adjunto
 participant B as P1
 A->>M: MPI_Buffer_attach · espacio suficiente + MPI_BSEND_OVERHEAD
 A->>M: MPI_Bsend · copia V[1,23,2,15,7,8]
 M-->>A: Retorno · V original reutilizable
 B->>B: MPI_Recv correspondiente
 M->>B: V[1,23,2,15,7,8]
 B->>B: Recv retorna con V completo
 A->>M: MPI_Buffer_detach
 M-->>A: Buffer devuelto cuando ya no está en uso
```

## MPI_Rsend

```mermaid
sequenceDiagram
 participant A as P0 · V[1,23,2,15,7,8]
 participant B as P1
 B->>B: MPI_Irecv · origen=0 · tag=0 · count=6
 Note over A,B: Recepción ya publicada antes de Rsend
 A->>B: MPI_Rsend · V[1,23,2,15,7,8] · tag=0
 A->>A: Retorno · buffer emisor reutilizable
 B->>B: MPI_Wait · V completo disponible
```

## MPI_Isend

```mermaid
sequenceDiagram
 participant A as P0 · V[1,23,2,15,7,8]
 participant M as MPI
 participant B as P1
 A->>M: MPI_Isend · V · destino=1 · count=6
 M-->>A: request · retorno sin esperar finalización
 Note over A: Trabajo independiente · no modificar V
 B->>M: MPI_Recv correspondiente
 M->>B: V[1,23,2,15,7,8]
 A->>M: MPI_Wait(request)
 M-->>A: Envío completado · V reutilizable
```

## MPI_Irecv

```mermaid
sequenceDiagram
 participant A as P0 · V[1,23,2,15,7,8]
 participant B as P1
 B->>B: MPI_Irecv · origen=0 · count=6
 B->>B: Retorno con request
 Note over B: Trabajo independiente · no leer ni modificar buffer receptor
 A->>B: V[1,23,2,15,7,8]
 B->>B: MPI_Wait(request)
 Note over B: Recepción completa · V[1,23,2,15,7,8] disponible
```

## MPI_Wait

```mermaid
flowchart TD
 A["MPI_Irecv · request<br/>V esperado: [1,23,2,15,7,8]"] --> B["MPI_Wait(request)"]
 B --> C{"¿Recepción completa?"}
 C -->|No| D["Espera bloqueado"]
 D --> C
 C -->|Sí| E["Retorno<br/>V[1,23,2,15,7,8] disponible"]
```

## MPI_Test

```mermaid
flowchart TD
 A["MPI_Irecv · request<br/>V esperado: [1,23,2,15,7,8]"] --> B["MPI_Test(request, flag)<br/>consulta sin bloquear"]
 B --> C{"flag"}
 C -->|0| D["Trabajo independiente<br/>no acceder al buffer receptor"]
 D --> B
 C -->|1| E["Recepción completa<br/>V[1,23,2,15,7,8] disponible"]
```


