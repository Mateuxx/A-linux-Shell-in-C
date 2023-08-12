#include <stdio.h> 

int main () {
    char command[100];

    while(1) {
        printf("Myshell> ");
        fgets (command, sizeof(command),stdin);
    }
}