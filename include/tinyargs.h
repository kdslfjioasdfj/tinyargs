#ifndef TINYARGS_TINYARGS_H
#define TINYARGS_TINYARGS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef _MSC_VER
#define TINYARGS_INTERNAL_RESTRICT __restrict
#elif defined(__cplusplus)
#define TINYARGS_INTERNAL_RESTRICT
#else
#define TINYARGS_INTERNAL_RESTRICT restrict
#endif // _MSC_VER || __cplusplus

#ifdef _WIN32
#ifdef TINYARGS_CONFIG_SHARED
#ifdef TINYARGS_CONFIG_BUILDING
#define TINYARGS_PUBLIC_API __declspec(dllexport)
#else
#define TINYARGS_PUBLIC_API __declspec(dllimport)
#endif // TINYARGS_CONFIG_BUILDING
#else
#define TINYARGS_PUBLIC_API // Not needed on Windows static builds (.lib)
#endif                      // TINYARGS_CONFIG_SHARED
#else
#define TINYARGS_PUBLIC_API // Not needed outside Windows
#endif                      // _WIN32

typedef void (*tinyargs_callback_t)(void *shared_ctx, void *private_ctx,
                                    const char *arg, const char **val);

typedef struct tinyargs_arg_s {
  const char *name;
  void *ctx;
  tinyargs_callback_t action;
  int n_args;
  uint8_t type;
} tinyargs_arg_t;

#define TINYARGS_TYPES_SHORT ((uint8_t)(1))
#define TINYARGS_TYPES_LONG ((uint8_t)(2))

#define TINYARGS_ESUCCESS ((int)0)
#define TINYARGS_EINVAL ((int)1)
#define TINYARGS_EFEW ((int)2)
#define TINYARGS_EUNKNOWN ((int)3)

TINYARGS_PUBLIC_API int
tinyargs_parse(const tinyargs_arg_t *TINYARGS_INTERNAL_RESTRICT args,
               size_t count, int argc, char **argv, void *shared_ctx,
               int *last);

#endif // TINYARGS_TINYARGS_H
