# TinyArgs Reference Manual

## Types

### tinyargs_callback_t

The callback format used by **TinyArgs**.

Definition:

```c
typedef void (*tinyargs_callback_t)(void *shared_ctx, void *private_ctx,
                                    const char *arg, const char **val);
```

Parameters:

- `void *shared_ctx`: This context pointer is passed to all callbacks
- `void *private_ctx`: This context pointer is passed only to the specific argument
- `const char *arg`: The name of the current argument
- `const char **val`: The arguments passed after the current one

### struct tinyargs_arg_s && tinyargs_arg_t

The argument schema used by **TinyArgs**.

Definition:

```c
typedef struct tinyargs_arg_s {
  const char *name;
  void *ctx;
  tinyargs_callback_t action;
  int n_args;
  uint8_t type;
} tinyargs_arg_t;
```

Members:

- `const char *name`: The name of the argument. Should NOT include prefix `-` or `--`. It is case-sensitive
- `void *ctx`: The private context to this argument
- `tinyargs_callback_t action`: The callback to be called when this argument is encountered
- `int n_args`: The arity of the argument. If `n_args > 0`, the callback will only be called if there are `n_args` arguments following the current argument. If `n_args` is 0, `const char **val` will be passed as `NULL`
- `uint8_t type`: The type of argument. It is a bitmask between `TINYARGS_TYPES_SHORT` and `TINYARGS_TYPES_LONG` (of which only one must be set). If both are defined, the function will return an error (`TINYARGS_EINVAL`)

Notes:
If `type & TINYARGS_TYPES_LONG` is true, `name` will be prefixed with `--` automatically.
If `type & TINYARGS_TYPES_SHORT` is true, `name` will be prefixed with `-` automatically.
Prefixing with `-` or `--` yourself is undefined behavior.

## Functions

### tinyargs_parse

Signature:

```c
int
tinyargs_parse(const tinyargs_arg_t *TINYARGS_INTERNAL_RESTRICT args,
               size_t count, int argc, char **argv, void *shared_ctx,
               int *last);
```

Parameters:

- `const tinyargs_arg_t *TINYARGS_INTERNAL_RESTRICT args`: The schema to follow while parsing arguments
- `size_t count`: The amount of definitions in `args`
- `argc`: The amount of arguments in `argv`
- `argv`: The argument vector
- `shared_ctx`: This will be passed to all callbacks upon successful matching
- `int *last`: Upon success, this is the first positional argument encountered. Upon error, the argument that caused the error. If unwanted, this can be `NULL`. If an invalid parameter was passed, this will be 1

Possible return values:
| Value | Meaning |
|-------|---------|
| `TINYARGS_ESUCCESS` | No error was encountered during parsing; all arguments parsed successfully |
| `TINYARGS_EINVAL` | An invalid argument was passed or an invalid argument existed in `args` |
| `TINYARGS_EFEW` | An argument expected more arguments than what was passed |
| `TINYARGS_EUNKNOWN` | An unknown argument was found |

Notes:

- If `(args[N]).name[0] == '-'`, for all `N` where `N < count`, the behavior is not defined.
- If any argument is called `--`, the parsing will immediately end, regardless of if said argument was defined in the schema.
- Flag grouping is NOT supported. `-abc` is not treated as `-a -b -c`.
- GNU-style assignment is NOT supported. `--value=VALUE` is not treated as `--value VALUE`.
- No automatic help printing is supported.
- A single hyphen `-` is treated as a positional argument.

## Disclaimers

1. Do not depend on any undocumented items; these are solely internal and not meant for usage
2. This library expects `argv` to end in `NULL`, where `argv[argc] == NULL`
3. This library assumes all strings are null-terminated (end in `\0`).

## Compatibility Notes

If you are linking with a shared library to use **TinyArgs** (presumably using `premake5 <system> --shared`), `TINYARGS_CONFIG_SHARED` must be defined, otherwise, there may be _linker errors_.
