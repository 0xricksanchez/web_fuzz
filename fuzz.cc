#include "server.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080
#define BUFFER_SIZE 1024

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  handle_request(-1, data, size);
  return 0;
}
