#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>



int main() {
    char input[1024];       // Buffer for user input
    char *command;          // Pointer to hold the command (e.g., "cd")
    char *argument;         // Pointer to hold the argument (e.g., "folder1")
    char cwd[1024];         // Buffer for the current working directory
    char history[1024][1024]; // Buffer for the history of commands
    int historyIndex = 0;  // Index to keep track of the history
    int historySize = 0;   // Size of the history buffer
    history[historyIndex][0] = '\0'; // Initialize the history buffer

    while (1) { // Infinite loop for the shell


        // Print the current working directory as a prompt
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s> ", cwd);
        } else {
            perror("getcwd error");
            continue;
        }

        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets error");
            continue;
        }

        // Remove the trailing newline from input
        input[strcspn(input, "\n")] = '\0';


        // Check if the user wants to use a command from history
        char myString[] = "!0"; // Buffer to hold the command number

        for(int i = 0; i < historyIndex; i++) {

            myString[1] = i + '0';

            if(strcmp(input, myString) == 0){
                strcpy(input, history[historyIndex-i-1]);
                input[strcspn(input, "\n")] = '\0';
                break;
            }
            
        }

        // Add the command to the history
        strcpy(history[historyIndex], input); // Copy input to history
        historyIndex = historyIndex + 1;
        history[historyIndex][0] = '\0';
        if (historySize > 100) {
            historyIndex = 0;
        }

        // Parse the command and argument
        command = strtok(input, " ");  // First token: the command
        argument = strtok(NULL, " "); // Second token: the argument

        if (command == NULL) {
            continue; // Empty command, loop again
        }

        if (strcmp(command, "cd") == 0) {
            if (argument == NULL) {
                fprintf(stderr, "cd: missing argument\n");
            } else {
                if (chdir(argument) != 0) {
                    perror("cd error");
                }
            }
        } else if (strcmp(command, "exit") == 0) {
            break;

        }else if(strcmp(command, "history") == 0) {
            if(historyIndex > 100) {
                for (int i = 99; i >= 0; i--) {
                printf("%s\n", history[i]);
            }
                
            }else{
            for (int i = historyIndex-1; i >= 0; i--) {
                printf("%s\n", history[i]);
                
            }
            }
        } else {
            //use execvp to run commands
            char *args[] = {command, argument, NULL};
            int pid = fork();
            if (pid == 0) {
                // child
                int myInt = execvp(args[0], args);
                printf("Command not found\n");
                
    
            } else {

                int status;

                int result = waitpid(-1, &status, 0);

            
        }
        
    }

    }

    printf("Shell exited\n");
    return 0;

}
