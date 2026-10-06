#ifndef MPI_EDITOR_ONLY_H
#define MPI_EDITOR_ONLY_H
#if !defined(__INTELLISENSE__) || !defined(_WIN32)
#error "Solo para IntelliSense Windows. Compila con MPI real en WSL."
#endif
/* Declaraciones educativas, no representan la ABI de MPI real. */
typedef int MPI_Comm;
typedef int MPI_Datatype;
typedef int MPI_Op;
typedef struct { int MPI_SOURCE, MPI_TAG, MPI_ERROR; } MPI_Status;
#define MPI_COMM_WORLD ((MPI_Comm)0)
#define MPI_INT ((MPI_Datatype)1)
#define MPI_SUM ((MPI_Op)1)
#define MPI_MAX ((MPI_Op)2)
#define MPI_MIN ((MPI_Op)3)
#define MPI_STATUS_IGNORE ((MPI_Status *)0)
int MPI_Init(int *, char ***);
int MPI_Finalize(void);
int MPI_Comm_rank(MPI_Comm, int *);
int MPI_Comm_size(MPI_Comm, int *);
int MPI_Send(const void *, int, MPI_Datatype, int, int, MPI_Comm);
int MPI_Recv(void *, int, MPI_Datatype, int, int, MPI_Comm, MPI_Status *);
int MPI_Abort(MPI_Comm, int);
int MPI_Bcast(void *, int, MPI_Datatype, int, MPI_Comm);
int MPI_Reduce(const void *, void *, int, MPI_Datatype, MPI_Op, int, MPI_Comm);
int MPI_Allreduce(const void *, void *, int, MPI_Datatype, MPI_Op, MPI_Comm);
int MPI_Gather(const void *, int, MPI_Datatype, void *, int, MPI_Datatype, int, MPI_Comm);
int MPI_Gatherv(const void *, int, MPI_Datatype, void *, const int *, const int *, MPI_Datatype, int, MPI_Comm);
int MPI_Scatter(const void *, int, MPI_Datatype, void *, int, MPI_Datatype, int, MPI_Comm);
int MPI_Scatterv(const void *, const int *, const int *, MPI_Datatype, void *, int, MPI_Datatype, int, MPI_Comm);
int MPI_Reduce_scatter(const void *, void *, const int *, MPI_Datatype, MPI_Op, MPI_Comm);
int MPI_Barrier(MPI_Comm);
int MPI_Scan(const void *, void *, int, MPI_Datatype, MPI_Op, MPI_Comm);
#endif
