#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* concat(char* s1, char* s2){
    if (s1 == NULL || s2 == NULL){
        return NULL;
    }
    size_t len = strlen(s1) + strlen(s2);
    char* result = malloc(len + 1);
    if (result == NULL){
        return NULL;
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int count_args(char** args){
    int num_args = 0;
    for (int i = 0; args[i] != NULL; i++){
        num_args++;
    }
    return num_args;
}

int is_whitespace(char* string){
    for (int i = 0; string[i] != '\0'; i++){
        if (string[i] != ' ' && string[i] != '\t'){
            return 0;
        }
    }
    return 1;
}

void print_error(){
    char error_message[30] = "An error has occurred\n";
    fprintf(stderr, "%s", error_message);
}

void print_args(char*** args, int num_commands){
    for (int i = 0; i < num_commands; i++){
        printf("Command %d: ", i);
        char** command_args = args[i];
        for (int j = 0; command_args[j] != NULL; j++){
            printf("%s, ", command_args[j]);
        }
        printf("\n");
    }
}