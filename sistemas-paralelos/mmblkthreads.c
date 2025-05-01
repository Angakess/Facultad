#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/*** VARIABLES GLOBALES (memoria compartida) ***/
double *a, *b, *c;
int n, bs, T;

/*** Prototipos ***/
void initvalmat(double *mat, int n, double val, int transpose);
void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs);
double dwalltime();
void *threadmult(void *arg);

/*** Función principal ***/
int main(int argc, char *argv[]) {
  if ((argc != 4) || ((n = atoi(argv[1])) <= 0) ||
      ((bs = atoi(argv[2])) <= 0) || ((n % bs) != 0) ||
      ((T = atoi(argv[3])) <= 0)) {
    printf("\nUso: %s N BS T (N debe ser múltiplo de BS)\n", argv[0]);
    exit(1);
  }

  a = (double *)malloc(n * n * sizeof(double));
  b = (double *)malloc(n * n * sizeof(double));
  c = (double *)malloc(n * n * sizeof(double));

  initvalmat(a, n, 1.0, 0);
  initvalmat(b, n, 1.0, 1);
  initvalmat(c, n, 0.0, 0);

  pthread_t threads[T];
  double timetick = dwalltime();

  for (int t = 0; t < T; t++) {
    int *tid = malloc(sizeof(int));
    *tid = t;
    pthread_create(&threads[t], NULL, threadmult, (void *)tid);
  }

  for (int t = 0; t < T; t++) {
    pthread_join(threads[t], NULL);
  }

  double time = dwalltime() - timetick;

  // Verificar resultado
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (c[i * n + j] != n) {
        printf("Error en (%d, %d): %f\n", i, j, c[i * n + j]);
      }
    }
  }

  printf("Tamaño %d, Bloques %d, Hilos %d -> Tiempo: %f segundos\n", n, bs, T,
         time);

  free(a);
  free(b);
  free(c);
  return 0;
}

/*** Función ejecutada por cada hilo ***/
void *threadmult(void *arg) {
  int tid = *((int *)arg);
  free(arg); // liberar memoria del tid

  int blocks = n / bs;

  for (int bi = tid; bi < blocks; bi += T) {
    for (int bj = 0; bj < blocks; bj++) {
      for (int bk = 0; bk < blocks; bk++) {
        blkmul(&a[(bi * bs) * n + bk * bs], &b[(bj * bs) * n + bk * bs], &c[(bi * bs) * n + bj * bs], n, bs);
      }
    }
  }

  pthread_exit(NULL);
}

/*** Inicialización de matrices ***/
void initvalmat(double *mat, int n, double val, int transpose) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (transpose)
        mat[j * n + i] = val;
      else
        mat[i * n + j] = val;
    }
  }
}

/*** Multiplicación de submatrices por bloques ***/
void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs) {
  for (int i = 0; i < bs; i++) {
    for (int j = 0; j < bs; j++) {
      for (int k = 0; k < bs; k++) {
        cblk[i * n + j] += ablk[i * n + k] * bblk[j * n + k];
      }
    }
  }
}

/*** Tiempo en segundos ***/
double dwalltime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}
