#ifndef ULTRACOMAN_CLI_H
#define ULTRACOMAN_CLI_H

#include "ultracoman/config.h"

typedef enum { CLI_OK, CLI_HELP, CLI_ERROR } CliResult;

CliResult cli_parse(int argc, char **argv, AppConfig *config);

void cli_print_usage(const char *program);

#endif
