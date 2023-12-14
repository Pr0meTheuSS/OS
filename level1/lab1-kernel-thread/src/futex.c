#include "futex.h"

void wait_on_futex_value(int* futex_addr, int val) {
  while (1) {
    if (*futex_addr == val) {
      return;
    }

    int futex_rc = syscall(SYS_futex, futex_addr, FUTEX_WAIT, val, NULL, NULL, 0);
    if (futex_rc == -1 && errno != EAGAIN) {
        perror("futex wait fail");
        exit(1);
    }
  }
}

void wake_futex_blocking(int* futex_addr, int val) {
  atomic_store(futex_addr, val);
  while (1) {
    int futex_rc = syscall(SYS_futex, futex_addr, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
    if (futex_rc == -1) {
      perror("futex wake error");
      exit(1);
    } else {
      return;
    }
  }
}
