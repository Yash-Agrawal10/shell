#pragma once

#include <sys/types.h>
#include "context.h"
#include "parser.h"

#define PID_ERROR -1
#define PID_BUILTIN -2

pid_t execute_nonbuiltin(ParsedCommand* parsed_command, ShellContext* context);

pid_t execute(ParsedCommand* parsed_command, ShellContext* context);

pid_t* execute_all(ParsedCommand** parsed_commands, int num_commands, ShellContext* context);

int wait_all(pid_t* pids, int num_commands);

int execute_and_wait_all(ParsedCommand** parsed_commands, int num_commands, ShellContext* context);