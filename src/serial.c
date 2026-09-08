#include "ultracoman/serial.h"

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

static int baud_to_speed(int baud, speed_t *speed) {
  switch (baud) {
  case 9600:
    *speed = B9600;
    return 0;

  case 19200:
    *speed = B19200;
    return 0;

  case 38400:
    *speed = B38400;
    return 0;

  case 57600:
    *speed = B57600;
    return 0;

  case 115200:
    *speed = B115200;
    return 0;

  default:
    errno = EINVAL;
    return -1;
  }
}

int serial_open(const SerialOptions *options) {
  int fd = open(options->device, O_RDWR | O_NOCTTY);

  if (fd < 0) {
    return -1;
  }

  struct termios tty;

  if (tcgetattr(fd, &tty) < 0) {
    goto error;
  }

  cfmakeraw(&tty);

  speed_t speed;

  if (baud_to_speed(options->baud, &speed) < 0) {
    goto error;
  }

  if (cfsetispeed(&tty, speed) < 0) {
    error;
  }

  if (cfsetospeed(&tty, speed) < 0) {
    goto error;
  }

  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;

  tty.c_cflag &= ~PARENB;

  tty.c_cflag &= ~CSTOPB;

  tty.c_cflag |= CREAD | CLOCAL;

#ifdef CRTSCTS
  tty.c_cflag &= ~CRTSCTS;
#endif /* ifdef CRTSCTS                                                        \
        */

  tty.c_cc[VMIN] = 1;
  tty.c_cc[VTIME] = 0;

  if (tcsetattr(fd, TCSANOW, &tty) < 0) {
    goto error;
  }

  return fd;

error: {
  int saved_error = errno;

  close(fd);

  errno = saved_error;
}

  return -1;
}
