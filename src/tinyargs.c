#include "tinyargs.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static inline bool validate_flags(uint8_t combination) {
  uint8_t mask = (TINYARGS_TYPES_SHORT | TINYARGS_TYPES_LONG);
  uint8_t bits = combination & mask;
  return bits && !(bits & (bits - 1));
}

#define HANDLE_SUCCESS 0
#define HANDLE_NOTENOUGHVALS 2
#define HANDLE_NOTMATCH 3

static inline int handle_cur_opt(char **argv, int argc, int cur,
                                 const tinyargs_arg_t *arg, void *shared_ctx) {
  if (!arg->name || argv[cur][0] != '-') {
    return HANDLE_NOTMATCH;
  }
  if (arg->type & TINYARGS_TYPES_SHORT) {
    if (argv[cur][0] == '\0' || argv[cur][1] == '\0' ||
        strcmp(argv[cur] + 1, arg->name) != 0)
      return HANDLE_NOTMATCH;
  } else {
    if (argv[cur][1] != '-' || argv[cur][2] == '\0' ||
        strcmp(argv[cur] + 2, arg->name) != 0)
      return HANDLE_NOTMATCH;
  }
  if ((argc - 1 - cur) < arg->n_args)
    return HANDLE_NOTENOUGHVALS;
  arg->action(shared_ctx, arg->ctx, arg->name,
              (arg->n_args > 0) ? (const char **)(argv + cur + 1) : NULL);

  return HANDLE_SUCCESS;
}

int tinyargs_parse(const tinyargs_arg_t *TINYARGS_INTERNAL_RESTRICT args,
                   size_t count, int argc, char **argv, void *shared_ctx,
                   int *last) {
  if (!args || count == 0 || argc <= 0 || !argv) {
    if (last)
      *last = 1;
    return TINYARGS_EINVAL;
  }
  for (size_t j = 0; j < count; j++) {
    if (!(validate_flags(args[j].type))) {
      if (last)
        *last = 1;
      return TINYARGS_EINVAL;
    }
  }
  int i = 1;
  int status = TINYARGS_ESUCCESS;
  while (i < argc) {
    if (strcmp(argv[i], "--") == 0) {
      i++;
      break;
    }
    if (argv[i][0] != '-' || argv[i][1] == '\0') {
      break;
    }
    bool found = false;
    for (size_t j = 0; j < count; j++) {
      int evaled = handle_cur_opt(argv, argc, i, &args[j], shared_ctx);

      if (evaled == HANDLE_SUCCESS) {
        i += (1 + args[j].n_args);
        found = true;
        break;
      } else if (evaled == HANDLE_NOTENOUGHVALS) {
        status = TINYARGS_EFEW;
        goto finish;
      }
    }
    if (!found) {
      status = TINYARGS_EUNKNOWN;
      goto finish;
    }
  }
finish:
  if (last)
    *last = i;
  return status;
}
