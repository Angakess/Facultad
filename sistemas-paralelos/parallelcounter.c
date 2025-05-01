#include <float.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define DEFAULT_SIZE 1000
#define DEFAULT_THREADS 100
#define DEFAULT_OBJECTIVE 1

int N = DEFAULT_SIZE;
int T = DEFAULT_THREADS;
int X = DEFAULT_OBJECTIVE;

double *vector;

int total = 0;
pthread_mutex_t total_lock;

/*** Tiempo en segundos ***/
double dwalltime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

/*** Función ejecutada por cada hilo ***/
void *threaded_counter(void *ptr) {
  int *p, id;
  p = (int *)ptr;
  id = *p;

  int size = (N + T - 1) / T;
  int start = id * size;
  int end = (id + 1) * size;
  int local_total = 0;

  for (int i = start; i < end && i < N; i++) {
    if (vector[i] == X) {
      local_total++;
    }
  }

  pthread_mutex_lock(&total_lock);
  total += local_total;
  pthread_mutex_unlock(&total_lock);

  pthread_exit((void *)ptr);
}

/*** Función principal ***/
int main(int argc, char *argv[]) {
  if ((argc != 4) || (N = atoi(argv[1]) <= 0)) {
    printf("Usar %s N X T", argv[0]);
    exit(1);
  }
  N = atoi(argv[1]);
  X = atoi(argv[2]);
  T = atoi(argv[3]);

  double timetick;
  int ids[DEFAULT_THREADS];
  pthread_attr_t attr;
  pthread_t threads[DEFAULT_THREADS];
  int *status;

  vector = (double *)malloc(sizeof(double) * N);

  for (int i = 0; i < N; i++) {
    vector[i] = (rand() % 5) + 1;
  }

  pthread_attr_init(&attr);
  pthread_mutex_init(&total_lock, NULL);

  timetick = dwalltime();
  for (int i = 0; i < T; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, threaded_counter, &ids[i]);
  }
  for (int i = 0; i < T; i++) {
    pthread_join(threads[i], (void *)&status);
    printf("\n Hilo %d retornó %d\n", i, (*status));
  }

  double time = dwalltime() - timetick;

  printf(
      "PARALELO: Se encontro el valor '%d' %d veces. Tiempo en segundos %f\n",
      X, total, time);

  timetick = dwalltime();

  total = 0;
  for (int i = 0; i < N; i++) {
    if (vector[i] == X) {
      total++;
    }
  }

  time = dwalltime() - timetick;

  printf(
      "SECUENCIAL: Se encontro el valor '%d' %d veces. Tiempo en segundos %f\n",
      X, total, time);
}
