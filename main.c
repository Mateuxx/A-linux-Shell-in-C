#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TOKENS 20 
#define MAX_TOKEN_LENGHT  //tamanho max para cada token



int parseEntrada (char *input, char **tokens){
    
    int numTokens = 0; //contador para saber quantos tokens foram encontrados
    char *token = strtok(input, " \t\n"); // " \t\n" -> espaço em branco, new line
    
    while (token!= NULL && numTokens < MAX_TOKENS){
        tokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " \t\n");
    } 
     return numTokens;
}


void executaComando(char **tokens) {

    pid_t pid = fork(); //criação do processo

    if (pid==0) {
        //Pricesso child
        execvp(tokens[0],tokens);
        perror("Erro no comando");
        exit(EXIT_FAILURE);
    }
    else if(pid > 0) {
        //processo pai
        wait(NULL); //espera até acabar o processo filho
    }
    else {
        perror ("Erro ao criar processo filho");
    }
}





int main () {
    
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
        printf("Numero de tokens: %d\n",numTokens);

        //printf("Todos os Tokens:\n");
         //for (int i = 0; i < numTokens; i++) {
          //printf("Token %d: %s\n", i, tokens[i]);
            //}
    }


     
    return 0;
}
