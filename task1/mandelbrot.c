// Задание 1. Параллельный поиск npoints точек множества Мандельброта
// Запуск:  ./program nthreads npoints
// Результат: файл mandelbrot.csv с координатами точек (x = Re c, y = Im c)
 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_rand.h"
#include "timer.h"

#define MAX_ITER 1000  
#define X_MIN -2.0     
#define X_MAX  0.5      
#define Y_MIN -1.25
#define Y_MAX  1.25


int thread_count;
long npoints;
double** points;
long* counts;     

int In_mandelbrot(double cx, double cy) {
   double x = 0.0, y = 0.0;          /* z_1 = 0 */
   for (int n = 0; n < MAX_ITER; n++) {
      double x2 = x * x, y2 = y * y;
      if (x2 + y2 >= 4.0)            /* |z| >= 2 */
         return 0;
      y = 2.0 * x * y + cy;          /* Im(z^2 + c) */
      x = x2 - y2 + cx;              /* Re(z^2 + c) */
   }
   return 1;                         
}

void* routine(void* rank) {
   long my_rank = (long) rank;
   long my_count = counts[my_rank];
   double* my_points = points[my_rank];
   unsigned seed = my_rank + 1;     
   long found = 0;

   while (found < my_count) {
      double cx = X_MIN + (X_MAX - X_MIN) * my_drand(&seed);
      double cy = Y_MIN + (Y_MAX - Y_MIN) * my_drand(&seed);
      if (In_mandelbrot(cx, cy)) {
         my_points[2 * found]     = cx;
         my_points[2 * found + 1] = cy;
         found++;
      }
   }
   return NULL;
}

int main(int argc, char** argv) {
   if (argc != 3) {
      fprintf(stderr, "usage: %s nthreads npoints\n", argv[0]);
      return 1;
   }
   thread_count = strtol(argv[1], NULL, 10);
   npoints = strtol(argv[2], NULL, 10);
   if (thread_count <= 0 || npoints <= 0) {
      fprintf(stderr, "nthreads и npoints должны быть > 0\n");
      return 1;
   }

   pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));
   points = malloc(thread_count * sizeof(double*));
   counts = malloc(thread_count * sizeof(long));

   
   for (long i = 0; i < thread_count; i++) {
      counts[i] = npoints / thread_count + (i < npoints % thread_count ? 1 : 0);
      points[i] = malloc((2 * counts[i] + 1) * sizeof(double));
   }

   double start, end;
   GET_TIME(start);
   for (long i = 0; i < thread_count; i++) {
      int err = pthread_create(&thread_handles[i], NULL, routine, (void*) i);
      if (err != 0)
         perror("Error");
   }
   for (int i = 0; i < thread_count; i++)
      pthread_join(thread_handles[i], NULL);
   GET_TIME(end);
   printf("%e\n", end - start);

 
   FILE* f = fopen("mandelbrot.csv", "w");
   if (f == NULL) {
      perror("mandelbrot.csv");
      return 1;
   }
   fprintf(f, "x,y\n");
   for (int i = 0; i < thread_count; i++)
      for (long j = 0; j < counts[i]; j++)
         fprintf(f, "%.10f,%.10f\n", points[i][2 * j], points[i][2 * j + 1]);
   fclose(f);

   for (int i = 0; i < thread_count; i++)
      free(points[i]);
   free(points);
   free(counts);
   free(thread_handles);
   return 0;
}
