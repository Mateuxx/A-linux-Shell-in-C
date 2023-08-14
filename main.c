#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TOKENS 20 
#define MAX_TOKEN_LENGTH 100

char initialPath[MAX_TOKEN_LENGTH]; // Variável global para armazenar o caminho inicial

// Função para obter o diretório atual e extrair a parte desejada para ficar algo semelhante a mateusnobrega@MANDVS323PCJ3:~/Desktop/Terminal---SO$
void diretorioCd(char *cwd) {
    if (getcwd(cwd, MAX_TOKEN_LENGTH) == NULL) {
        perror("Erro ao obter diretório atual");
        exit(EXIT_FAILURE);
    }

    char *substring = strstr(cwd, initialPath);
    if (substring != NULL) {
        strcpy(cwd, substring + strlen(initialPath)); // Copie a parte desejada para o caminho atual
    }
}


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

void comandoCd(char *dir) {
    if (chdir(dir) == 0) {
        //printf("Diretório alterado para: %s\n", dir);
    } else {
        perror("Erro ao alterar o diretório");
    }
}


void executaComando(char **tokens) {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Processo filho
        execvp(tokens[0], tokens);
        // Erro ao executar o comando, sair com código de erro
        perror("Comando não encontrado");
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
    char cwd[MAX_TOKEN_LENGTH];

    if (getcwd(initialPath, MAX_TOKEN_LENGTH) == NULL) {
        perror("Erro ao obter diretório inicial");
        return 1;
    }

    while (1) {
        diretorioCd(cwd);  // Obtém o diretório atual a partir de Terminal---SO para o comanodo cd
        printf("ShellSO>:%s$ ", cwd);      // Exibe o prompt com o diretório atual

        fgets(command, sizeof(command), stdin);

        if (command[strlen(command) - 1] == '\n') {
            command[strlen(command) - 1] = '\0';
        }

        numTokens = parseEntrada(command, tokens);

        if (numTokens > 0) {
        if (strcmp(tokens[0], "cd") == 0) { //cd -> comando interno do proprio shell
            if (numTokens > 1) {
                comandoCd(tokens[1]);
            } else {
                printf("\nVoce quis dizer:\n\ncd <diretório>\n\n");
            }
        
        } else if (strcmp(tokens[0], "ls") == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                execvp(tokens[0], tokens); //execvp() - recebe o nome de um arquivo (na mesma pasta) e os argumentos.
                perror("ls");
                exit(1);
            } else {
                waitpid(pid, NULL, 0);
            }
        } else if (strcmp(tokens[0], "pwd") == 0) {
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        } else if (strcmp(tokens[0], "exit") == 0) { 
            break;
        }else { //execução dos progamas executaveis -> programas separadas - comandos do shell do linux
            executaComando(tokens);
        }
    }


    }
    return 0;

}