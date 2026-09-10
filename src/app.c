#include "ultracoman/app.h"
#include "ultracoman/cli.h"
#include "ultracoman/config.h"
#include "ultracoman/event_loop.h"
#include "ultracoman/serial.h"
#include "ultracoman/terminal.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int app_run(int argc, char **argv) {
  AppConfig config;

  CliResult cli_result = cli_parse(argc, argv, &config);

  if (cli_result == CLI_HELP) {
    cli_print_usage(argv[0]);
    return 0;
  }

  if (cli_result == CLI_ERROR) {
    cli_print_usage(argv[0]);
    return 1;
  }

  int serial_fd = serial_open(&config.serial);

  if (serial_fd < 0) {
    fprintf(stderr, "%s: %s\n", config.serial.device, strerror(errno));

    return 1;
  }

  if (terminal_enter_raw() < 0) {
    int saved_errno = errno;

    close(serial_fd);

    fprintf(stderr, "terminal: %s\n", strerror(saved_errno));

    return 1;
  }

  int result = event_loop_run(serial_fd, config.quit_key);

  int saved_errno = errno;

  terminal_restore();
  close(serial_fd);

  if (result < 0) {
    fprintf(stderr, "ultracoman: %s\n", strerror(saved_errno));

    return 1;
  }

  return 0;
}
