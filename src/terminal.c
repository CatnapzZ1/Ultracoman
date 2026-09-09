#include "ultracoman/terminal.h"

#include <termios.h>
#include <unistd.h>

static struct termios saved_terminal;
static int raw_mode_enabled = 0;

int terminal_enter_raw(void) {
  if(!isatty(STDIN_FILENO){
    return 0;
  }

  if(tcgetattr(STDIN_FILENO, &saved_terminal) < 0){
    return -1;
  }

  struct termios raw = saved_terminal;

  cfmakeraw(&raw);

  if(tcsetattr(STDIN_FILENO, TCSANOW, &raw) < 0){
    return -1;
  }

  raw_mode_enabled = 1;

  return 0;
}

void terminal_restre(void) {
  if (!raw_mode_enabled) {
    return;
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &saved_terminal);

  raw_mode_enabled = 0;
}
