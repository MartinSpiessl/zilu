#include <klee/klee.h>


int main() {
int i;
int j;
int k;
klee_make_symbolic(&i, sizeof(i), "i");
klee_make_symbolic(&j, sizeof(j), "j");
klee_make_symbolic(&k, sizeof(k), "k");
klee_pass(k>=0 && k<=100 && i>=0 && i<=k && j==0);
return 0;
}
