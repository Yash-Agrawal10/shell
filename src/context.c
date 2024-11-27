#include "context.h"
#include "builtins.h"
#include <stdlib.h>
#include <string.h>

ShellContext* initialize_context(){
    ShellContext* context = malloc(sizeof(ShellContext));
    if (context == NULL){
        return NULL;
    }

    context->path_length = 1;
    char** path = malloc(context->path_length * sizeof(char*));
    if (path == NULL){
        free(context);
        return NULL;
    }
    path[0] = strdup("/bin/");
    context->path = path;

    context->builtins_length = 3;
    BuiltinCommand* builtins = malloc(context->builtins_length * sizeof(BuiltinCommand));
    if (builtins == NULL){
        for (int i = 0; i < context->path_length; i++){
            free(context->path[i]);
            free(context->path);
        }
        free(context);
        return NULL;
    }
    builtins[0].command = strdup("exit");
    builtins[0].func = wexit;
    builtins[1].command = strdup("cd");
    builtins[1].func = wcd;
    builtins[2].command = strdup("path");
    builtins[2].func = wpath;
    context->builtins = builtins;
    return context;
}

void free_context(ShellContext* context){
    for (int i = 0; i < context->path_length; i++){
        free(context->path[i]);
    }
    free(context->path);
    for (int i = 0; i < context->builtins_length; i++){
        free(context->builtins[i].command);
    }
    free(context->builtins);
    free(context);
}