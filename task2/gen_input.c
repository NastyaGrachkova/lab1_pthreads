/* Генератор входного файла для nbody.
 * Запуск: ./gen_input n filename
 * Первая строка — число тел, дальше n строк:
 *   масса  x  y  z  vx  vy  vz
 */
#include <stdio.h>
#include <stdlib.h>
#include "my_rand.h"

static double uniform(unsigned* seed, double a, double b) {
    return a + (b - a) * my_drand(seed);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s n filename\n", argv[0]);
        fprintf(stderr, "n — число тел\n");
        fprintf(stderr, "пример: %s 100 demo.txt\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "n must be > 0\n");
        return 1;
    }

    FILE* f = fopen(argv[2], "w");
    if (f == NULL) {
        perror(argv[2]);
        return 1;
    }

    unsigned seed = 42;
    fprintf(f, "%d\n", n);
    for (int i = 0; i < n; i++) {
        double mass = uniform(&seed, 1e20, 1e24);
        double x = uniform(&seed, -1e11, 1e11);
        double y = uniform(&seed, -1e11, 1e11);
        double z = uniform(&seed, -1e11, 1e11);
        double vx = uniform(&seed, -1e3, 1e3);
        double vy = uniform(&seed, -1e3, 1e3);
        double vz = uniform(&seed, -1e3, 1e3);
        fprintf(f, "%.16e %.16e %.16e %.16e %.16e %.16e %.16e\n",
                mass, x, y, z, vx, vy, vz);
    }

    fclose(f);
    printf("Generated %d bodies in %s\n", n, argv[2]);
    return 0;
}
