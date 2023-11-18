#include "futex.h"

void wait_on_futex_value(int* futex_addr, int val) {
  while (1) {
    int futex_rc = syscall(SYS_futex, futex_addr, FUTEX_WAIT, val, NULL, NULL, 0);
    if (futex_rc == -1) {
      if (errno != EAGAIN) {
        perror("futex");
        exit(1);
      }
    } else if (futex_rc == 0) {
      if (atomic_compare_exchange_strong(futex_addr, &val, val)) {
        return;
      }
    } else {
      abort();
    }
  }
}

void wake_futex_blocking(int* futex_addr) {
  while (1) {
    int futex_rc = syscall(SYS_futex, futex_addr, FUTEX_WAKE, 1, NULL, NULL, 0);
    if (futex_rc == -1) {
      perror("futex wake");
      exit(1);
    } else if (futex_rc > 0) {
      return;
    }
  }
}
