#include <float.h>
#include <pthread.h>
#include <semaphore.h>
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

int max, min;
double avg;
// sem_t sem_min, sem_max, sem_avg;
sem_t sem_scalars;

/*** Tiempo en segundos ***/
double dwalltime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

/*** Función ejecutada por cada hilo ***/
void *threaded_scalars(void *ptr) {
  int *p, id;
  p = (int *)ptr;
  id = *p;

  int size = (N + T - 1) / T;
  int start = id * size;
  int end = (id + 1) * size;
  int local_max = vector[start];
  int local_min = vector[start];
  double local_sum = 0;

  for (int i = start; i < end && i < N; i++) {
    if (vector[i] < local_min) {
      local_min = vector[i];
    }
    if (vector[i] > local_max) {
      local_max = vector[i];
    }
    local_sum += vector[i];
  }

  sem_wait(&sem_scalars);
  if (local_max > max) {
    max = local_max;
  }
  if (local_min < min) {
    min = local_min;
  }
  avg = avg + local_sum;
  sem_post(&sem_scalars);

  pthread_exit((void *)ptr);
}

/*** Función principal ***/
int main(int argc, char *argv[]) {
  if ((argc != 3) || (N = atoi(argv[1]) <= 0)) {
    printf("Usar %s N T", argv[0]);
    exit(1);
  }
  N = atoi(argv[1]);
  T = atoi(argv[2]);

  double timetick;
  int ids[DEFAULT_THREADS];
  pthread_attr_t attr;
  pthread_t threads[DEFAULT_THREADS];
  int *status;

  vector = (double *)malloc(sizeof(double) * N);

  for (int i = 0; i < N; i++) {
    vector[i] = (rand() % N) + 1;
    // printf("\n %f", vector[i]);
  }
  max = vector[0];
  min = vector[0];
  avg = 0;

  pthread_attr_init(&attr);
  sem_init(&sem_scalars, 0, 1);

  timetick = dwalltime();
  for (int i = 0; i < T; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, threaded_scalars, &ids[i]);
  }
  for (int i = 0; i < T; i++) {
    pthread_join(threads[i], (void *)&status);
    printf("\n Hilo %d retornó %d\n", i, (*status));
  }
  avg = avg / N;

  double time = dwalltime() - timetick;

  printf("PARALELO: Min: %d - Max: %d - Avg: %f. Tiempo en segundos %f\n", min,
         max, avg, time);

  timetick = dwalltime();

  min = vector[0];
  max = vector[0];
  avg = 0;
  for (int i = 0; i < N; i++) {
    if (vector[i] < min) {
      min = vector[i];
    }
    if (vector[i] > max) {
      max = vector[i];
    }
    avg = avg + vector[i];
  }
  avg = avg / N;

  time = dwalltime() - timetick;

  printf("SECUENCIAL: Min: %d - Max: %d - Avg: %f. Tiempo en segundos %f\n",
         min, max, avg, time);
}
