#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

/**
 * Disable raw mode by restoring original state of STDIN_FILENO
 */
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/**
 * Enable raw mode by switching off the c_lflag attribute of the terminal's
 * STDIN_FILENO
 */
void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode); // Run when program terminates

  // Don't print keys to terminal when they are pressed (only switch bit that
  // is turned on in ECHO
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  char c;
  // Read 1 byte at a time from stdin to variable c until no bytes are left
  // (read returns 0) or a 'q' is inputted
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}
