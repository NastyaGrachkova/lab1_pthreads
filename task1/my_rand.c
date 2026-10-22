#include <stdio.h>
#include <stdlib.h>
#include "my_rand.h"

#define MR_MULTIPLIER 279470273 
#define MR_INCREMENT 0
#define MR_MODULUS 4294967291U
#define MR_DIVISOR ((double) 4294967291U)


#ifdef _MAIN_
int main(void) {
   int n, i; 
   unsigned seed = 1, x;
   double y;

   printf("How many random numbers?\n");
   scanf("%d", &n);

   x = my_rand(&seed);
   for (i = 0; i < n; i++) {
      x = my_rand(&x);
      printf("%u\n", x);
   }
   for (i = 0; i < n; i++) {
      y = my_drand(&x);
      printf("%e\n", y);
   }
   return 0;
}
#endif

unsigned my_rand(unsigned* seed_p) {
   long long z = *seed_p;
   z *= MR_MULTIPLIER; 
// z += MR_INCREMENT;
   z %= MR_MODULUS;
   *seed_p = z;
   return *seed_p;
}


double my_drand(unsigned* seed_p) {
   unsigned x = my_rand(seed_p);
   double y = x/MR_DIVISOR;
   return y;
}
