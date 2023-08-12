#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_CMD_LENGTH 100

void executeCommand(char *command, char *args[], char *inputFile, char *outputFile, int background, int pipeFlag, char *pipeCommand[]) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process

        // Handle input redirection
        if (inputFile != NULL) {
            freopen(inputFile, "r", stdin);
        }

        // Handle output redirection
        if (outputFile != NULL) {
            freopen(outputFile, "w", stdout);
        }

        // Handle pipe
        if (pipeFlag) {
            int pipefd[2];
            pipe(pipefd);
            pid_t pipeChild = fork();

            if (pipeChild == 0) {
                // Child process for the first command in pipe
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                execvp(command, args);
                perror("Error executing command");
                exit(EXIT_FAILURE);
            } else {
                // Parent process
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                execvp(pipeCommand[0], pipeCommand);
                perror("Error executing command");
                exit(EXIT_FAILURE);
            }
        }

        // Normal command execution
        execvp(command, args);
        perror("Error executing command");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        if (!background) {
            waitpid(pid, NULL, 0);
        }
    } else {
        perror("Fork failed");
    }
}

int main() {
    char input[MAX_CMD_LENGTH];
    while (1) {
        printf("Shell> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        char *args[MAX_ARGS];
        char *pipeCommand[MAX_ARGS];
        char *token;
        char *rest = input;
        int pipeFlag = 0;

        // Parse the input command and arguments
        int argCount = 0;
        while ((token = strtok_r(rest, " ", &rest))) {
            if (strcmp(token, "|") == 0) {
                pipeFlag = 1;
                argCount = 0;
                continue;
            }

            if (!pipeFlag) {
                args[argCount] = token;
            } else {
                pipeCommand[argCount] = token;
            }
            
            argCount++;

            if (argCount >= MAX_ARGS) {
                break;
            }
        }

        if (argCount == 0) {
            continue;
        }

        args[argCount] = NULL;
        pipeCommand[argCount] = NULL;

        // Handle built-in commands
        if (strcmp(args[0], "exit") == 0) {
            exit(0);
        } else if (strcmp(args[0], "ls") == 0) {
            executeCommand("/bin/ls", args, NULL, NULL, 0, pipeFlag, pipeCommand);
        } else if (strcmp(args[0], "cd") == 0) {
            if (argCount < 2) {
                fprintf(stderr, "Usage: cd <directory>\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("cd error");
                }
            }
        } else if (strcmp(args[0], "pwd") == 0) {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Current working directory: %s\n", cwd);
            } else {
                perror("getcwd error");
            }
        } else {
            int background = 0;
            char *inputFile = NULL;
            char *outputFile = NULL;

            // Check for background execution
            if (argCount > 1 && strcmp(args[argCount - 1], "&") == 0) {
                background = 1;
                args[argCount - 1] = NULL;
            }

            // Check for input and output redirection
            for (int i = 1; i < argCount; i++) {
                if (strcmp(args[i], ">") == 0) {
                    if (i + 1 < argCount) {
                        outputFile = args[i + 1];
                        args[i] = NULL;
                        args[i + 1] = NULL;
                        break;
                    } else {
                        fprintf(stderr, "Missing output file name\n");
                        break;
                    }
                } else if (strcmp(args[i], "<") == 0) {
                    if (i + 1 < argCount) {
                        inputFile = args[i + 1];
                        args[i] = NULL;
                        args[i + 1] = NULL;
                        break;
                    } else {
                        fprintf(stderr, "Missing input file name\n");
                        break;
                    }
                }
            }

            // Execute the command
            executeCommand(args[0], args, inputFile, outputFile, background, pipeFlag, pipeCommand);
        }
    }

    return 0;
}
