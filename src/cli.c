#include "ultracoman/cli.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_baud(const char *text, int *baud) {
  char *end;

  errno = 0;

  long value = strtol(text, &end, 10);

  if (errno != 0 || *end != '\0' || value <= 0 || value > INT_MAX) {
    return -1;
  }

  *baud = (int)value;

  return 0;
}

void cli_print_usage(const char *program) {
  fprintf(stderr,
          "Usage:\n"
          " %s DEVICE BAUD\n"
          "\n"
          "Example:\n"
          " %s /dev/ttyUSB0 115200\n"
          "\n"
          "Quit:\n"
          " Ctrl+]\n",
          program, program);
}

CliResult cli_parse(int argc, char **argv, AppConfig *config) {
  config->serial.device = NULL;
  config->serial.baud = 115200;
  config->quit_key = 0x1d;

  if (argc == 2 &&
      (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
    return CLI_HELP;
  }

  if (argc != 3) {
    return CLI_ERROR;
  }

  config->serial.device = argv[1];

  if (parse_baud(argv[2], &config->serial.baud) < 0) {
    return CLI_ERROR;
  }

  return CLI_OK;
}
