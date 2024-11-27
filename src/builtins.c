#include "builtins.h"
#include "utils.h"
#include "context.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int wexit(char** args, ShellContext* context){
    (void) context;
    int num_args = count_args(args);
    if (num_args != 1){
        return -1;
    }
    free_context(context);
    exit(0);
}

int wcd(char** args, ShellContext* context){
    (void) context;
    int num_args = count_args(args);
    if (num_args != 2){
        return -1;
    }
    if (chdir(args[1]) == -1){
        return -1;
    }
    return 0;
}

char** get_new_path(char** args, int* length){
    int num_args = count_args(args);
    int new_path_length = num_args - 1;
    char** new_path = malloc(new_path_length * sizeof(char*));
    if (new_path == NULL){
        return NULL;
    }
    for (int i = 0; i < new_path_length; i++){
        char* path_with_slash = concat(args[i + 1], "/");
        new_path[i] = path_with_slash;
    }
    *length = new_path_length;
    return new_path;
}

int wpath(char** args, ShellContext* context){
    int new_path_length = 0;
    char** new_path = get_new_path(args, &new_path_length);
    if (new_path == NULL){
        return -1;
    }
    if (context->path != NULL) free (context->path);
    context->path = new_path;
    context->path_length = new_path_length;
    return 0;
}

int is_builtin(char* command, ShellContext* context){
    for (int i = 0; i < context->builtins_length; i++){
        if (strcmp(command, context->builtins[i].command) == 0){
            return 1;
        }
    }
    return 0;
}

int handle_builtin(char** args, ShellContext* context){
    for (int i = 0; i < context->builtins_length; i++){
        if (strcmp(args[0], context->builtins[i].command) == 0){
            return context->builtins[i].func(args, context);
        }
    }
    return -1;
}