#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TOKENS 20 
#define MAX_TOKEN_LENGTH 100

int parseEntrada(char *input, char **tokens) {
    int numTokens = 0;
    char *token = strtok(input, " \t\n");

    while (token != NULL && numTokens < MAX_TOKENS) {
        tokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " \t\n");
    }

    tokens[numTokens] = NULL; // Marca o final da lista de tokens
    return numTokens;
}

void executaComando(char **tokens) {
    pid_t pid = fork();

    if (pid == 0) {
        // Processo filho
        execvp(tokens[0], tokens);
        perror("Erro ao executar o comando");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Processo pai
        wait(NULL);
    } else {
        perror("Erro ao criar o processo filho");
    }
}

int main() {
    char command[100];
    char *tokens[MAX_TOKENS];
    int numTokens;

    while (1) {
        printf("MyShell> ");
        fgets(command, sizeof(command), stdin);

        if (command[strlen(command) - 1] == '\n') {
            command[strlen(command) - 1] = '\0';
        }

        numTokens = parseEntrada(command, tokens);

        if (numTokens > 0) {
            executaComando(tokens);
        }
    }

    return 0;
}
