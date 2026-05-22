# Minishell Bug Report V2

## Overview
This bug report covers the most serious issues found in the current codebase. It focuses on crashes, resource leaks, incorrect behavior, and bad signal handling.

---

## 1. Heredoc input handling (crash + leak)

### Files
- `helper_functions/tool_box_heredoc.c`
- `execution/heredoc_management.c`

### Issues
- `read_hd()` calls `ft_strncmp(line, curr->file, ...)` even when `readline()` returns `NULL`.
  - Result: segmentation fault on EOF / Ctrl+D inside heredoc.
- `read_hd()` can exit the loop with an allocated `line` and never free it.
  - Result: memory leak on heredoc exit.
- `handle_heredoc()` opens a pipe and may not close `fd[1]` in all error paths.
  - Result: file descriptor leak.

### Recommendation
- Add `if (line == NULL)` handling immediately after `readline()`.
- Free `line` before breaking from the heredoc loop.
- Ensure `fd[1]` is closed on all early exits or errors.

---

## 2. Fork and pipe error handling (logic crash)

### Files
- `execution/execution.c`

### Issues
- `execute_system_command()` does not check the return value of `fork()`.
  - If `fork()` returns `-1`, `p->children[p->i]` is `-1`.
- `wait_children()` loops over all commands and calls `waitpid(child[i], ...)`.
  - If `child[i] == -1`, this is invalid and may wait on the wrong process or behave unpredictably.
- `execute_system_command()` also does not check the return value of `pipe(p->curr)`.

### Recommendation
- Add error handling for `pipe()` failure.
- Check `fork()` return value, and avoid storing or waiting on `-1`.
- Stop execution and clean up properly on process creation failure.

---

## 3. Signal handling and global state

### Files
- `helper_functions/tool_box_parsing.c`
- `execution/heredoc_management.c`
- `helper_functions/tool_box_heredoc.c`

### Issues
- `g_status` is declared as `int g_status = 0;` and modified in signal handlers.
  - This is unsafe; it should be `volatile sig_atomic_t`.
- `heredoc_sigint()` and `handle_sigint()` both write `g_status` without atomic safety.

### Recommendation
- Change `g_status` to `volatile sig_atomic_t g_status;` in the header and source.
- Use only async-signal-safe operations inside signal handlers.

---

## 4. `exit_program()` and `kill_child()` closing std FDs

### Files
- `main.c`
- `execution/exits.c`

### Issues
- Both `exit_program()` and `kill_child()` close `STDIN_FILENO`, `STDOUT_FILENO`, and `STDERR_FILENO` immediately before `exit()`.
- Closing standard FDs is unnecessary and can cause problems if any cleanup or error printing runs after the close.

### Recommendation
- Remove explicit closures of file descriptors 0, 1, and 2 before exiting.

---

## 5. Environment conversion and allocation failures

### Files
- `helper_functions/tool_box_env.c`

### Issues
- `envp_list_to_arr()` does not check result of first `ft_strjoin()`.
- If `temp` allocation fails, the function dereferences NULL and crashes.

### Recommendation
- Check `temp` and `arr[i]` for NULL and free all allocated memory before returning on failure.

---

## 6. Incomplete heredoc scanning

### Files
- `execution/heredoc_management.c`

### Issues
- `check_heredocs()` returns immediately after processing the first heredoc in the command chain.
- If a later command or later redirection also uses heredoc, it is not processed.

### Recommendation
- Do not `return` inside the command loop when encountering the first heredoc.
- Continue scanning all heredocs so every here-document is handled.

---

## 7. Built-in environment manipulation safety

### Files
- `built_ins/export.c`
- `built_ins/export.c`
- `built_ins/unset.c`

### Issues
- `add_to_env()` does not check `malloc()` or `ft_strdup()` failures.
- `process_arg()` duplicates text into `key` and `value`, but if `replace_env_value()` fails internally, it can leak.
- `remove_env()` is OK, but `ft_unset()` does not report invalid identifiers.

### Recommendation
- Add allocation failure handling in `add_to_env()`.
- Return failure status from export/unset when necessary.

---

## 8. `cd` path handling and message formatting

### Files
- `built_ins/cd.c`

### Issues
- Uses a fixed 1024-byte `cwd` buffer instead of `PATH_MAX`.
- The error message for too many arguments is missing the `minishell: cd:` prefix.

### Recommendation
- Use `PATH_MAX` with `#include <limits.h>`.
- Print consistent builtin error messages.

---

## 9. Potential token parsing / quote bugs

### Files
- `parsing/ini_token_1.c`
- `parsing/ini_cmd.c`

### Issues
- Unclosed quotes produce an error, but token extraction may return partially allocated data in some failure paths.
- `build_redir_node()` assumes `(*cur)->next` exists without extra safety.
- `create_args()` splits unquoted tokens containing spaces; this can be okay, but edge cases on empty tokens may be mishandled.

### Recommendation
- Add stronger NULL checks around token-node assumptions.
- Consider tightening word-splitting logic so it matches shell rules exactly.

---

## 10. Minor style / safety issues

- `check_file_descriptors()` only tests `> 2`, but should treat `-1` carefully.
- `go_to_last_working_dir()` prints `OLDPWD` but does not change shell state if `chdir()` later fails.
- `handle_heredoc()` uses `write(1, "\n", 1)` instead of stderr.

---

## Recommended Priority Fix Order
1. `read_hd()` null dereference and heredoc leak
2. `fork()`/`pipe()` error handling in `execute_system_command()`
3. Change `g_status` to `volatile sig_atomic_t`
4. Fix `exit_program()` / `kill_child()` FD closure
5. Harden `envp_list_to_arr()` allocations
6. Fix `check_heredocs()` early return logic
7. Add allocation checks in builtin env operations
8. Replace fixed `cwd` buffer with `PATH_MAX`

---

## Notes
This codebase has multiple issues but the most urgent ones are the heredoc crash and the missing `fork()` error check. Those are the most likely causes of the unstable behavior you described.
