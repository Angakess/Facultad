#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define MAX_THREADS 100
#define SIZE 256

int T = MAX_THREADS;
int N = SIZE;

double *A, *B, *C;
void *add_in_thread(void *ptr);

// Para calcular tiempo
double dwalltime() {
  double sec;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  sec = tv.tv_sec + tv.tv_usec / 1000000.0;
  return sec;
}

int main(int argc, char *argv[]) {
  int i;
  int check = 1;
  double timetick;
  int ids[MAX_THREADS];
  pthread_attr_t attr;
  pthread_t threads[MAX_THREADS];
  int *status;

  // Controla los argumentos al programa
  if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0)) {
    printf("\nUsar: %s n t\n  n: Dimension de la matriz (nxn X nxn)\n  t: "
           "Cantidad de threads\n",
           argv[0]);
    exit(1);
  }

  // Aloca memoria para los vectores
  A = (double *)malloc(sizeof(double) * N);
  B = (double *)malloc(sizeof(double) * N);
  C = (double *)malloc(sizeof(double) * N);

  // Inicializa los vectores en 1, el resultado será una vector con todos sus
  // valores en 2
  for (i = 0; i < N; i++)
    A[i] = 1;
  for (i = 0; i < N; i++)
    B[i] = 1;

  // Realiza la multiplicacion

  timetick = dwalltime();

  pthread_attr_init(&attr);
  for (i = 0; i < T; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, add_in_thread, &ids[i]);
  }
  for (i = 0; i < T; i++) {
    pthread_join(threads[i], (void *)&status);
  }

  double time = dwalltime() - timetick;

  printf("Suma de vectores de dimensión %d. Tiempo en segundos %f\n", N, time);

  // Verifica el resultado
  for (i = 0; i < N; i++)
    check = check && (C[i] == 2);

  if (check) {
    printf("Suma de vectores resultado correcto\n");
  } else {
    printf("Suma de vectores resultado erroneo\n");
  }

  free(A);
  free(B);
  free(C);
  return (0);
}

void *add_in_thread(void *ptr) {
  int *p, id, i;
  p = (int *)ptr;
  id = *p;

  int t_size = (N + (T - 1)) / T;
  for (i = t_size * id; i < (t_size * id + t_size) && i < N; i++) {
    C[i] = A[i] + B[i];
  }

  pthread_exit((void *)ptr);
}
