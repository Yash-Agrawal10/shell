#pragma once

// Utility

char* concat(char* s1, char* s2);

int count_args(char** args);

int is_whitespace(char* string);

// Debugging

void print_error();

void print_args(char*** args, int num_commands);