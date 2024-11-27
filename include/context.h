#pragma once

typedef struct ShellContext ShellContext;

typedef struct BuiltinCommand {
    char* command;
    int (*func)(char** args, ShellContext* context);
} BuiltinCommand;

typedef struct ShellContext{
    char** path;
    int path_length;
    BuiltinCommand* builtins;
    int builtins_length;
} ShellContext;

ShellContext* initialize_context();

void free_context(ShellContext* context);