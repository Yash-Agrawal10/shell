#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "utils.h"
#include "executor.h"
#include "context.h"

int main(int argc, char* argv[]){
    // Parse command line inputs
    FILE* input;
    char* msg = "";
    if (argc == 1){
        // Interactive Mode
        input = stdin;
        msg = "wish> ";
    }
    else if (argc == 2){
        // Batch Mode
        input = fopen(argv[1], "r");
        if (input == NULL){
            print_error();
            exit(1);
        }
    }
    else{
        print_error();
        exit(1);
    }

    // Initialize context
    ShellContext* context = initialize_context();
    if (context == NULL){
        print_error();
        exit(1);
    }

    // Run shell loop
    while (1){
        printf("%s", msg);

        // Parse input
        int error = 0;
        int num_commands = 0;
        ParsedCommand** parsed_commands = parse(input, &num_commands, &error);
        if (error) {
            print_error();
            continue;
        }

        // Print args for debugging
        // print_args(args, num_commands);

        // Execute commands
        if (execute_and_wait_all(parsed_commands, num_commands, context) == -1){
            print_error();
            continue;
        }
    }
}