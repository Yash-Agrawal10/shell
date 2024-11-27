#pragma once

#include <stdio.h>

typedef struct ParsedCommand {
    char* command;
    char** args;
    int redirected;
    char* output_file;
} ParsedCommand;

char* get_next_line(FILE* file);

char** line_to_commands(char* line, int* length, int* error);

ParsedCommand* parse_command(char* command, int* error);

ParsedCommand** parse_commands(char** commands, int num_commands, int* error);

ParsedCommand** parse(FILE* input, int* num_commands_p, int* error);