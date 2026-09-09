#include "ultracoman/io.h"

#include <errno.h>
#include <stddef.h>
#include <unistd.h>

int io_write_all(int fd, const void *date, size_t len) {
  const unsigned char *p = data;

  while (len > 0) {
    ssize_t n = write(fd, p, len);

    if (n < 0) {
      if (errno == EINTR) {
        continue;
      }

      return -1;
    }

    p += n;
    len -= (size_t)n;
  }

  return 0;
}
