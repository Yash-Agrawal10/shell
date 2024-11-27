#include "parser.h"
#include "utils.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

char* get_next_line(FILE* input){
    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    linelen = getline(&line, &linecap, input);
    if (linelen < 0){
        return NULL;
    }
    line[linelen - 1] = '\0';
    return line;
}

char** line_to_commands(char* line, int* length, int* error){
    *error = 0;
    char** commands = NULL;
    int num_commands = 0;
    char* token;
    char* string;
    string = line;
    while ((token = strsep(&string, "&")) != NULL){
        if (is_whitespace(token)){
            continue;
        }
        num_commands++;
        char** temp = realloc(commands, num_commands * sizeof(char*));
        if (temp == NULL){
            if (commands != NULL) free(commands);
            *error = 1;
            return NULL;
        }
        commands = temp;
        commands[num_commands - 1] = strdup(token);
    }
    *length = num_commands;
    return commands;
}

ParsedCommand* parse_command(char* command, int* error){
    *error = 0;
    // Split command into arguments and redirect strings
    char* redirect_string = command;
    char* arg_string = strsep(&redirect_string, ">");
    if (strlen(arg_string) == 0){
        *error = 1;
        return NULL;
    }

    // Split argumment string into arguments
    char** args = NULL;
    char* token;
    int num_args = 0;
    while ((token = strsep(&arg_string, " \t")) != NULL){
        if (strlen(token) != 0){
            num_args++;
            char** temp = realloc(args, num_args * sizeof(char*));
            if (temp == NULL){
                *error = 1;
                if (args != NULL) free(args);
                return NULL;
            }
            args = temp;
            args[num_args - 1] = strdup(token);
        }
    }
    num_args++;
    char** temp = realloc(args, num_args * sizeof(char*));
    if (temp == NULL){
        *error = 1;
        free(args);
        return NULL;
    }
    args = temp;
    args[num_args - 1] = NULL;

    // Process redirect string
    char* output_file = NULL;
    int redirected = 0;
    if (redirect_string != NULL){
        output_file = NULL;
        while ((token = strsep(&redirect_string, " \t")) != NULL){
            if (strlen(token) != 0){
                if (output_file != NULL){
                    *error = 1;
                    free(args);
                    free(output_file);
                    return NULL;
                }
                else{
                    output_file = strdup(token);
                    redirected = 1;
                }
            }
        }
        if (output_file == NULL){
            *error = 1;
            free(args);
            return NULL;
        }
    }
    
    // Parse tokens into ParsedCommand
    ParsedCommand* parsed_command = malloc(sizeof(ParsedCommand));
    parsed_command->args = args;
    parsed_command->command = args[0];
    parsed_command->redirected = redirected;
    parsed_command->output_file = output_file;
    return parsed_command;
}

ParsedCommand** parse_commands(char** commands, int num_commands, int* error){
    ParsedCommand** parsed_commands = malloc(num_commands * sizeof(ParsedCommand*));
    if (parsed_commands == NULL){
        return NULL;
    }
    for (int i = 0; i < num_commands; i++){
        parsed_commands[i] = parse_command(commands[i], error);
        if (*error){
            return NULL;
        }
    }
    return parsed_commands;
}

ParsedCommand** parse(FILE* input, int* num_commands_p, int* error){
    *error = 0;
    char* line;
    if ((line = get_next_line(input)) == NULL){
        // EOF
        exit(0);
    }
    char** commands = line_to_commands(line, num_commands_p, error);
    if (*error){
        free(line);
        return NULL;
    }
    free(line);
    ParsedCommand** parsed_commands;
    parsed_commands = parse_commands(commands, *num_commands_p, error);
    if (*error){
        if (commands != NULL) free(commands);
        return NULL;
    }
    if (commands != NULL) free(commands);
    return parsed_commands;
}