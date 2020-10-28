#include <assert.h>
void reach_error(void) {assert(0);}

extern int __VERIFIER_nondet_int(void);
extern _Bool __VERIFIER_nondet_bool(void);

void __VERIFIER_assert(int cond) {
  if (!cond) {
    reach_error();
  }
}

$(comment)
int main() {$(init)$(symbolicinit)$(beforeloopinit)$(precondition)
  while ($(loopcondition)) {
    $(loop)
  }
  __VERIFIER_assert($(postcondition));
  return 0;
}
