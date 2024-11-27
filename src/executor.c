#include "executor.h"
#include "utils.h"
#include "context.h"
#include "builtins.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

pid_t execute_nonbuiltin(ParsedCommand* parsed_command, ShellContext* context){
    char** path = context->path;
    for (int i = 0; i < context->path_length; i++){
        char* full_path = concat(path[i], parsed_command->command);
        if (full_path == NULL){
            continue;
        }
        if (access(full_path, X_OK) == 0){
            pid_t rc = fork();
            if (rc < 0){
                return PID_ERROR;
            }
            else if (rc == 0){
                if (parsed_command->redirected){
                    FILE* file = fopen(parsed_command->output_file, "w");
                    if (file == NULL){
                        exit(1);
                    }
                    int fd = fileno(file);
                    dup2(fd, STDOUT_FILENO);
                    fclose(file);
                }
                if (execv(full_path, parsed_command->args) == -1){
                    exit(1);
                }
            }
            else{
                return rc;
            }
        }
    }
    return PID_ERROR;
}

pid_t execute(ParsedCommand* parsed_command, ShellContext* context){
    if (is_builtin(parsed_command->command, context)){
        if (handle_builtin(parsed_command->args, context) == -1){
            return PID_ERROR;
        }
        return PID_BUILTIN;
    }
    else{
        return execute_nonbuiltin(parsed_command, context);
    }
}


pid_t* execute_all(ParsedCommand** parsed_commands, int num_commands, ShellContext* context){
    pid_t* pids = malloc(num_commands * sizeof(pid_t));
    if (pids == NULL){
        return NULL;
    }
    for (int i = 0; i < num_commands; i++){
        pids[i] = execute(parsed_commands[i], context);
    }
    return pids;
}

int wait_all(pid_t* pids, int num_commands){
    int ret = 0;
    for (int i = 0; i < num_commands; i++){
        if (pids[i] == PID_ERROR){
            ret = -1;
            continue;
        }
        else if (pids[i] == PID_BUILTIN){
            continue;
        }
        else{
            waitpid(pids[i], NULL, 0);
        }
    }
    return ret;
}

int execute_and_wait_all(ParsedCommand** parsed_commands, int num_commands, ShellContext* context){
    pid_t* pids = execute_all(parsed_commands, num_commands, context);
    if (pids == NULL){
        return -1;
    }
    return wait_all(pids, num_commands);
}