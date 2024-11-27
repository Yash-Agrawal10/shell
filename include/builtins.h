#pragma once

#include "context.h"

int wexit(char** args, ShellContext* context);

int wcd(char** args, ShellContext* context);

char** get_new_path(char** args, int* length);

int wpath(char** args, ShellContext* context);

int is_builtin(char* command, ShellContext* context);

int handle_builtin(char** args, ShellContext* context);