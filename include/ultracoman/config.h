#ifndef ULTRACOMAN_CONFIG_H
#define ULTRACOMAN_CONFIG_H

typedef struct {
  const char *device;
  int baud;
} SerialOptions;

typedef struct {
  SerialOptions serial;
  unsigned char quit_key;
} AppConfig;
#endif // !ULTRACOMAN_CONFIG_H
