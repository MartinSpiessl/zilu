extern void __assert_fail (__const char *__assertion, __const char *__file,
      unsigned int __line, __const char *__function)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));
extern int __VERIFIER_nondet_int(void);
extern _Bool __VERIFIER_nondet_bool(void);

void reach_error(void) {__assert_fail ("0", "$(file)", 11, __PRETTY_FUNCTION__);}

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
