#include<stdio.h>
#include<string.h>

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
