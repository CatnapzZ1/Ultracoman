#include "ultracoman/event_loop.h"
#include "ultracoman/io.h"

#include <asm-generic/errno-base.h>
#include <errno.h>
#include <poll.h>
#include <stddef.h>
#include <sys/poll.h>
#include <unistd.h>

int event_loop_run(int serial_fd, unsigned char quit_key) {
  struct pollfd fds[2];

  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  fds[1].fd = serial_fd;
  fds[1].events = POLLIN;

  for (;;) {
    int result = poll(fds, 2, -1);

    if (result < 0) {
      if (errno == EINTR) {
        continue;
      }

      return -1;
    }

    if (fds[0].revents & POLLIN) {
      unsigned char buf[4096];

      ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));

      if (n < 0) {
        if (errno == EINTR) {
          continue;
        }
        return -1;
      }

      if (n == 0) {
        return 0;
      }

      for (ssize_t i = 0; i < n; i++) {
        if (buf[i] == quit_key) {
          return 0;
        }

        if (io_write_all(serial_fd, &buf[i], 1) < 0) {
          return -1;
        }
      }
    }

    if (fds[1].revents & POLLIN) {
      unsigned char buf[4096];

      ssize_t n = read(serial_fd, buf, sizeof(buf));

      if (n < 0) {
        if (errno == EINTR) {
          continue;
        }

        return -1;
      }

      if (n == 0) {
        return 0;
      }

      if (io_write_all(STDOUt_FILENO, buf, (size_t)n) < 0) {
        return -1;
      }
    }

    if (fds[1].revents & (POLLERR | POLLHUP | POLLNVAL)) {
      return 0;
    }
  }
}
