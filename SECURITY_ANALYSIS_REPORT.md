# Minishell Security & Memory Safety Analysis Report
**Date:** May 17, 2026  
**Status:** Comprehensive Code Review Complete

---

## Executive Summary
This report identifies **18 security and memory safety issues** across the minishell project. Issues range from **critical** (potential crashes, data loss) to **low** (code quality). Most critical issues involve:
- Null pointer dereferences in heredoc handling
- Memory leaks in error paths
- Unsafe string modifications
- Missing error checks on system calls

---

## Issues Found

### 1. NULL Pointer Dereference in Heredoc Reading
**File:** [execution/heredoc_management.c](execution/heredoc_management.c#L52-L63)  
**Severity:** CRITICAL  
**Type:** Use-After-Free / Null Pointer Dereference

**Issue:**
```c
void read_hd(t_shell *shell, t_redir *curr, char *line, int fd[2])
{
    while (1)
    {
        line = readline("> ");
        if (g_status == 130
            || ft_strncmp(line, curr->file, ft_strlen(curr->file) + 1) == 0)
            break ;
```

`readline()` returns NULL on EOF, but the code immediately calls `ft_strncmp()` without checking if `line` is NULL. This causes a crash.

**Impact:** Program crash when EOF is encountered during heredoc input.

**Fix:**
```c
void read_hd(t_shell *shell, t_redir *curr, char *line, int fd[2])
{
    while (1)
    {
        line = readline("> ");
        if (line == NULL || g_status == 130)
            break ;
        if (ft_strncmp(line, curr->file, ft_strlen(curr->file) + 1) == 0)
            break ;
```

---

### 2. Memory Leak in readline() Failure Path
**File:** [execution/heredoc_management.c](execution/heredoc_management.c#L67-L74)  
**Severity:** MEDIUM  
**Type:** Memory Leak

**Issue:**
```c
int handle_heredoc(t_shell *shell, t_redir *curr)
{
    int     fd[2];
    char    *line;

    line = NULL;
    if (pipe(fd) == -1)
        return (perror("minishell: pipe"), -1);
    // ... later ...
    read_hd(shell, curr, line, fd);
    if (line)
        free(line);
    else if (g_status != 130)
        heredoc_error_msg(curr);
```

If `readline()` fails or returns NULL in `read_hd()`, the `line` variable remains NULL. Multiple calls to `readline()` may allocate memory internally that isn't freed.

**Impact:** Accumulated memory leaks when heredoc receives EOF or signals.

**Fix:**
Properly track all readline allocations and free them in the while loop of `read_hd()`.

---

### 3. Unsafe In-Place String Modification
**File:** [built_ins/export.c](built_ins/export.c#L28-L40)  
**Severity:** HIGH  
**Type:** Buffer Overflow / Memory Corruption

**Issue:**
```c
void process_arg(t_shell *shell, char *arg)
{
    t_env   *target_node;
    char    *equal_sign;
    char    *key;
    char    *value;

    equal_sign = ft_strchr(arg, '=');
    if (!equal_sign)
        return ;
    *equal_sign = '\0';  // <-- DANGEROUS: Modifies arg array
```

The function modifies `arg` (which comes from `cmd->args`) in-place. The `args` array may be read-only or used elsewhere in the program. This violates const-correctness and can cause undefined behavior.

**Impact:** Potential memory corruption if `arg` is part of a read-only data segment or shared between commands.

**Fix:**
```c
void process_arg(t_shell *shell, char *arg)
{
    t_env   *target_node;
    char    *arg_copy;
    char    *equal_sign;
    char    *key;
    char    *value;

    arg_copy = ft_strdup(arg);
    if (!arg_copy)
        return;
    equal_sign = ft_strchr(arg_copy, '=');
    if (!equal_sign)
        return (free(arg_copy));
    *equal_sign = '\0';
    key = arg_copy;
    value = equal_sign + 1;
    // ... rest of function
    free(arg_copy);
}
```

---

### 4. Unfreed Path Allocation in execute_command()
**File:** [execution/execution.c](execution/execution.c#L24-L48)  
**Severity:** MEDIUM  
**Type:** Memory Leak

**Issue:**
```c
void execute_command(t_shell *shell, t_cmd *cmd)
{
    // ...
    if (ft_strchr(cmd->args[0], '/'))
        path = ft_strdup(cmd->args[0]);
    else
        path = get_path(cmd->args[0], envp_arr);
    if (!path)
        exit_no_path(shell, envp_arr, cmd);
    if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
        exit_no_access(shell, path, envp_arr);
    if (execve(path, cmd->args, envp_arr) == -1)
        exit_execve_failure(shell, envp_arr, cmd);
}
```

When `exit_no_path()` is called, `path` is not freed before exiting. Similarly, `path` is not freed on successful `execve()` (though the process exits), but exit functions should still clean up.

**Impact:** Memory leak on command not found errors.

**Fix:**
```c
void execute_command(t_shell *shell, t_cmd *cmd)
{
    // ...
    if (!path) {
        free(path);  // Add this
        exit_no_path(shell, envp_arr, cmd);
    }
```

---

### 5. Memory Leak in readline() Unclosed Quote Error Path
**File:** [parsing/ini_token_1.c](parsing/ini_token_1.c#L15-L28)  
**Severity:** MEDIUM  
**Type:** Memory Leak

**Issue:**
```c
char *extract_d_quote(t_shell *shell, char *input, int *i)
{
    // ...
    final = ft_strdup("");
    // ... processing ...
    if (input[*i] == '\0')
        return (shell->exit_status = 2,
            free(final), printf("error: unclosed quote\n"), NULL);  // Freed here
    return ((*i)++, final);
}
```

While this function does free `final` on error, the `extract_quote()` function calls it:

```c
char *extract_quote(t_shell *shell, char *input, int *i)
{
    // ...
    else
        final = extract_d_quote(shell, input, i);
    return (final);
}
```

If `extract_d_quote()` returns NULL, `extract_quote()` returns NULL immediately without cleaning up any parent context. When `extract_token()` calls `extract_quote()`, it needs to handle NULL properly.

**Impact:** Parser may leak memory and fail to report unclosed quotes correctly in piped commands.

**Fix:** Ensure all quote parsing errors properly propagate and clean up in `build_token()`.

---

### 6. Missing Error Check for fork() System Call
**File:** [execution/execution.c](execution/execution.c#L63-L67)  
**Severity:** HIGH  
**Type:** Unchecked System Call

**Issue:**
```c
void execute_system_command(t_shell *shell, t_cmd *curr, t_pipe *p)
{
    if (curr->next)
        pipe(p->curr);
    p->children[p->i] = fork();
    if (p->children[p->i] == 0)  // Only checks for child process
    {
        // ...
    }
```

No check for `fork() == -1` (failure). The program doesn't detect when fork fails and continues as if child was created.

**Impact:** Undefined behavior if process limit is reached; commands may fail silently.

**Fix:**
```c
void execute_system_command(t_shell *shell, t_cmd *curr, t_pipe *p)
{
    if (curr->next)
        pipe(p->curr);
    p->children[p->i] = fork();
    if (p->children[p->i] == -1) {
        perror("fork");
        exit_fd_failure(shell, curr, p);
    }
    if (p->children[p->i] == 0)
    {
        // ...
    }
```

---

### 7. Unchecked pipe() System Call
**File:** [execution/execution.c](execution/execution.c#L62-L63)  
**Severity:** MEDIUM  
**Type:** Unchecked System Call

**Issue:**
```c
void execute_system_command(t_shell *shell, t_cmd *curr, t_pipe *p)
{
    if (curr->next)
        pipe(p->curr);  // No error checking!
```

If `pipe()` fails, `p->curr` contains -1 values, which are then used with `dup2()`, causing failures.

**Impact:** Pipe creation failures are silently ignored, leading to undefined behavior.

**Fix:**
```c
void execute_system_command(t_shell *shell, t_cmd *curr, t_pipe *p)
{
    if (curr->next) {
        if (pipe(p->curr) == -1) {
            perror("pipe");
            exit_program(shell, 1);
        }
    }
```

---

### 8. File Descriptor Check Inconsistency
**File:** [helper_functions/tool_box_fd.c](helper_functions/tool_box_fd.c#L29-L42)  
**Severity:** LOW  
**Type:** Code Quality / Potential Bug

**Issue:**
```c
void close_if_non_standard_in_out_file(int *infile, int *outfile)
{
    if (infile && *infile > 2)  // Checks > 2
    {
        close(*infile);
        *infile = -1;
    }
    if (outfile && *outfile > 2)  // Checks > 2
    {
        close(*outfile);
        *outfile = -1;
    }
}
```

But in [execution/fd_management.c](execution/fd_management.c#L7-L9):
```c
int manage_append(t_cmd *cmd, t_redir *curr)
{
    if (cmd->outfile > 1)  // Checks > 1 (!!)
        close (cmd->outfile);
```

Inconsistent FD threshold (1 vs 2). Standard FDs are 0,1,2 (stdin, stdout, stderr), so `> 2` is correct, but `> 1` is wrong as it could close stdout.

**Impact:** Accidental closure of stdout file descriptor.

**Fix:**
```c
int manage_append(t_cmd *cmd, t_redir *curr)
{
    if (cmd->outfile > 2)  // Should be > 2, not > 1
        close (cmd->outfile);
```

---

### 9. Direct Close of Standard File Descriptors
**File:** [main.c](main.c#L36-L40)  
**Severity:** MEDIUM  
**Type:** Resource Management Issue

**Issue:**
```c
void exit_program(t_shell *shell, int exit_code)
{
    // ... cleanup ...
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    exit (exit_code);
}
```

The function directly closes file descriptors 0, 1, 2 which is unnecessary and dangerous. The OS will close them anyway, and this can cause errors if they're already closed or if signals arrive during close.

**Impact:** Potential errors logged to closed stderr; defensive error handling disabled.

**Fix:**
```c
void exit_program(t_shell *shell, int exit_code)
{
    // ... cleanup ...
    // Don't close standard FDs - OS will handle them
    exit (exit_code);
}
```

---

### 10. File Descriptor Close Without Validation
**File:** [main.c](main.c#L27-L34)  
**Severity:** MEDIUM  
**Type:** File Descriptor Management

**Issue:**
```c
void exit_program(t_shell *shell, int exit_code)
{
    if (shell)
    {
        check_backups(shell);
        if (shell->cmds)
        {
            if (shell->cmds->outfile > 2)
                close(shell->cmds->outfile);
            if (shell->cmds->infile > 2)
                close(shell->cmds->infile);
```

Only the first command's FDs are closed. If there are multiple piped commands, only the first one's file descriptors are handled. The rest leak.

**Impact:** File descriptor leaks in piped commands.

**Fix:**
```c
void exit_program(t_shell *shell, int exit_code)
{
    if (shell)
    {
        check_backups(shell);
        if (shell->cmds)
            free_cmds(&(shell->cmds));  // This already closes FDs for all commands
```

---

### 11. getcwd() Buffer Overflow Potential
**File:** [built_ins/cd.c](built_ins/cd.c#L8-L20)  
**Severity:** MEDIUM  
**Type:** Buffer Overflow / Hardcoded Buffer Size

**Issue:**
```c
int go_to_new_dir(t_shell *shell, char *new_path)
{
    char    cwd[1024];  // Hardcoded size!
    t_env   *target_node;

    if (!getcwd(cwd, 1024))
        return (1);
```

Uses hardcoded buffer of 1024 bytes. Modern systems can have paths longer than this (especially with symlinks or deep nesting). POSIX specifies `PATH_MAX` should be used.

**Impact:** Potential buffer overflow if current working directory path exceeds 1024 bytes.

**Fix:**
```c
#include <limits.h>
int go_to_new_dir(t_shell *shell, char *new_path)
{
    char    cwd[PATH_MAX];  // Use PATH_MAX
    t_env   *target_node;

    if (!getcwd(cwd, PATH_MAX))
        return (1);
```

---

### 12. Same Buffer Overflow in pwd.c
**File:** [built_ins/pwd.c](built_ins/pwd.c#L14-L16)  
**Severity:** MEDIUM  
**Type:** Buffer Overflow / Hardcoded Buffer Size

**Issue:**
```c
int print_cwd(t_shell *shell, char **args)
{
    char    cwd[1024];  // Hardcoded size
```

Same issue as cd.c - hardcoded 1024 byte buffer.

**Fix:** Use `PATH_MAX` instead of hardcoded 1024.

---

### 13. Missing Null Pointer Check in getenv Variable Expansion
**File:** [helper_functions/tool_box_exec.c](helper_functions/tool_box_exec.c#L9-L26)  
**Severity:** MEDIUM  
**Type:** Null Pointer Dereference

**Issue:**
```c
char *get_path(char *command, char **envp)
{
    char    *path_str;
    char    **paths;
    char    *full_path;
    char    *temp;
    int     i;

    path_str = env_path(envp);
    if (!path_str || path_str[0] == '\0')
        return (NULL);
    paths = ft_split(path_str, ':');  // No null check!
```

If `ft_split()` fails (returns NULL), then `paths` is NULL and the subsequent `while (paths[i])` crashes.

**Impact:** Program crash when malloc fails during PATH parsing.

**Fix:**
```c
    paths = ft_split(path_str, ':');
    if (!paths)
        return (NULL);
    i = 0;
    while (paths[i])
```

---

### 14. Missing Return Value Check on malloc
**File:** [parsing/ini_env.c](parsing/ini_env.c#L68-L73)  
**Severity:** MEDIUM  
**Type:** Unchecked malloc

**Issue:**
```c
t_env *create_envp_node(char *envp_str)
{
    t_env   *cur;
    int     sep;

    cur = malloc(sizeof(t_env));
    if (cur == NULL)
        return (NULL);
```

While this checks malloc, if any `ft_strdup()` or `ft_substr()` call fails, memory is leaked:

```c
    else
    {
        cur->key = ft_substr(envp_str, 0, sep);
        cur->value = ft_substr(envp_str, sep + 1,
                (ft_strlen(envp_str) - sep - 1));
    }
    if (!cur->key || !cur->value)
        return (free(cur->key), free(cur->value), free(cur), NULL);
```

The cleanup is here, but the pattern is fragile.

**Impact:** Possible memory leaks if allocation fails mid-structure.

---

### 15. Readline Input Not Validated Before Use
**File:** [main.c](main.c#L69-L77)  
**Severity:** MEDIUM  
**Type:** Null Pointer Dereference

**Issue:**
```c
while (1)
{
    if (g_status != 0)
    {
        shell.exit_status = g_status;
        g_status = 0;
    }
    input = readline("minishell$ ");
    if (input == NULL)
        break ;
    if (input[0] != '\0')  // Could be dereferenced right after NULL check
        add_history(input);
    process_input(&shell, input);
    free(input);  // input could still be NULL theoretically after above blocks
}
```

While the code checks for NULL, the following line immediately accesses `input[0]`. If `readline()` is interrupted by a signal between NULL check and array access, this could theoretically fail (though unlikely).

**Impact:** Minor - mostly safe as written, but signal race condition possible.

---

### 16. Uninitialized Variable in export.c
**File:** [built_ins/export.c](built_ins/export.c#L25-L48)  
**Severity:** LOW  
**Type:** Uninitialized Variable / Logic Error

**Issue:**
```c
int ft_export(t_shell *shell, char **args)
{
    int i;
    int exit_status;

    exit_status = 0;
    if (!args[1])
        return (show_env(shell->env_list));
    i = 1;
    while (args[i])
    {
        if (is_valid_arg(args[i]))
            process_arg(shell, args[i]);
        else
        {
            ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
            exit_status = 1;
        }
        i++;
    }
    return (exit_status);
}
```

The function modifies `args[i]` in `process_arg()` by setting a null terminator at the `=` sign. Since this modifies argv entries, it persists across commands and could cause subsequent commands to have corrupted arguments.

**Impact:** Argument corruption for subsequent commands after export.

---

### 17. Missing Validation in envp_list_to_arr()
**File:** [helper_functions/tool_box_env.c](helper_functions/tool_box_env.c#L49-L68)  
**Severity:** MEDIUM  
**Type:** Memory Leak on Error

**Issue:**
```c
char **envp_list_to_arr(t_shell *shell)
{
    char    **arr;
    t_env   *curr;
    int     i;
    char    *temp;

    curr = shell->env_list;
    arr = malloc(sizeof(char *) * (envp_size(curr) + 1));
    if (!arr)
        return (NULL);
    i = 0;
    while (curr)
    {
        temp = ft_strjoin(curr->key, "=");
        arr[i] = ft_strjoin(temp, curr->value);  // If this fails, temp leaks
        free(temp);
        i++;
        curr = curr->next;
    }
```

If `ft_strjoin(temp, curr->value)` fails and returns NULL, `temp` is freed but then `arr[i]` is set to NULL. The function continues and returns `arr` with NULL entries that should have been there. Also, previously allocated strings in `arr` are not freed on this error.

**Impact:** Memory leak of partial environment array and temp strings on allocation failure.

**Fix:**
```c
char **envp_list_to_arr(t_shell *shell)
{
    char    **arr;
    t_env   *curr;
    int     i;
    char    *temp;

    curr = shell->env_list;
    arr = malloc(sizeof(char *) * (envp_size(curr) + 1));
    if (!arr)
        return (NULL);
    i = 0;
    while (curr)
    {
        temp = ft_strjoin(curr->key, "=");
        if (!temp) {
            free_array(arr);
            return (NULL);
        }
        arr[i] = ft_strjoin(temp, curr->value);
        free(temp);
        if (!arr[i]) {
            free_array(arr);
            return (NULL);
        }
        i++;
        curr = curr->next;
    }
    arr[i] = NULL;
    return (arr);
}
```

---

### 18. Race Condition in Signal Handler
**File:** [tool_box_parsing.c](helper_functions/tool_box_parsing.c#L18-L26)  
**Severity:** MEDIUM  
**Type:** Signal Safety Violation

**Issue:**
```c
void handle_sigint(int sig)
{
    (void)sig;
    g_status = 130;
    write(1, "\n", 1);
    rl_on_new_line();        // NOT signal-safe!
    rl_replace_line("", 0);   // NOT signal-safe!
    rl_redisplay();           // NOT signal-safe!
}
```

The signal handler calls readline functions (`rl_on_new_line()`, `rl_replace_line()`, `rl_redisplay()`) which are not async-signal-safe. These functions may allocate memory, call non-safe functions, etc.

**Impact:** Potential corruption, deadlocks, or undefined behavior when SIGINT arrives during execution.

**Fix:**
```c
void handle_sigint(int sig)
{
    (void)sig;
    g_status = 130;
    write(1, "\n", 1);
    // Don't call readline functions here - only signal-safe functions
    // Let the main loop handle readline state recovery
}
```

---

## Summary Table

| # | File | Issue | Severity | Type |
|---|------|-------|----------|------|
| 1 | execution/heredoc_management.c | NULL pointer dereference in readline | CRITICAL | Crash |
| 2 | execution/heredoc_management.c | Memory leak in readline failure | MEDIUM | Leak |
| 3 | built_ins/export.c | Unsafe in-place string modification | HIGH | Memory corruption |
| 4 | execution/execution.c | Unfreed path allocation | MEDIUM | Leak |
| 5 | parsing/ini_token_1.c | Memory leak on unclosed quote | MEDIUM | Leak |
| 6 | execution/execution.c | Missing fork() error check | HIGH | Silent failure |
| 7 | execution/execution.c | Missing pipe() error check | MEDIUM | Undefined behavior |
| 8 | execution/fd_management.c | FD check inconsistency | LOW | Logic error |
| 9 | main.c | Unnecessary closing of std FDs | MEDIUM | Resource error |
| 10 | main.c | Only first cmd's FDs closed | MEDIUM | FD leak |
| 11 | built_ins/cd.c | Buffer overflow potential (cwd) | MEDIUM | Overflow |
| 12 | built_ins/pwd.c | Buffer overflow potential (cwd) | MEDIUM | Overflow |
| 13 | helper_functions/tool_box_exec.c | Missing null check on ft_split | MEDIUM | Crash |
| 14 | parsing/ini_env.c | Unchecked malloc failure | MEDIUM | Leak |
| 15 | main.c | Readline race condition | MEDIUM | Race |
| 16 | built_ins/export.c | Argument corruption | LOW | Logic error |
| 17 | helper_functions/tool_box_env.c | Memory leak on allocation failure | MEDIUM | Leak |
| 18 | helper_functions/tool_box_parsing.c | Non-signal-safe in handler | MEDIUM | Corruption |

---

## Recommendations by Priority

### CRITICAL (Fix Immediately):
1. **Issue #1**: Fix NULL pointer dereference in heredoc reading - this will crash the shell

### HIGH (Fix Before Release):
2. **Issue #3**: Fix unsafe string modification in export command
3. **Issue #6**: Add fork() error checking

### MEDIUM (Fix Soon):
4. All memory leaks and file descriptor management issues
5. Buffer overflow vulnerabilities with PATH_MAX
6. Signal safety violations

### LOW (Quality Improvements):
7. Consistency checks and code quality improvements

