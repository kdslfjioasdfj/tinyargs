#include "tinyargs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  const char *input_file;
  int verbosity;
  bool force;
} app_context_t;

void set_input(void *shared, void *priv, const char *name, const char **val) {
  (void)shared;
  (void)priv;
  (void)name;
  app_context_t *ctx = (app_context_t *)shared;
  ctx->input_file = val[0];
}

void set_flag(void *shared, void *priv, const char *name, const char **val) {
  (void)shared;
  (void)priv;
  (void)name;
  (void)val;
  bool *flag = (bool *)priv;
  *flag = true;
}

void set_verbosity(void *shared, void *priv, const char *name,
                   const char **val) {
  (void)priv;
  (void)name;
  (void)val;
  app_context_t *ctx = (app_context_t *)shared;
  ctx->verbosity++;
}

int main(int argc, char **argv) {
  app_context_t app = {
      .input_file = "default.txt", .verbosity = 0, .force = false};

  const tinyargs_arg_t schema[] = {
      {"i", NULL, set_input, 1, TINYARGS_TYPES_SHORT},
      {"input", NULL, set_input, 1, TINYARGS_TYPES_LONG},
      {"v", NULL, set_verbosity, 0, TINYARGS_TYPES_SHORT},
      {"f", &app.force, set_flag, 0, TINYARGS_TYPES_SHORT},
      {"force", &app.force, set_flag, 0, TINYARGS_TYPES_LONG}};

  int result = tinyargs_parse(schema, 5, argc, argv, &app, NULL);

  switch (result) {
  case TINYARGS_ESUCCESS:
    break;
  case TINYARGS_EINVAL:
    puts("Got an invalid argument somewhere in arguments or the schema.");
    return 1;
  case TINYARGS_EFEW:
    puts("Some argument got too few values.");
    return 1;
  case TINYARGS_EUNKNOWN:
    puts("Got some unknown flags");
    return 1;
  default:
    puts("Got some unidentifiable error.");
    return 1;
  }

  printf("File: %s\nLevel: %d\nForce: %s\n", app.input_file, app.verbosity,
         app.force ? "Yes" : "No");

  return 0;
}
