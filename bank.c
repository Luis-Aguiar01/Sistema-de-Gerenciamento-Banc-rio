#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"

typedef struct user {
    char name[50];
    char login[30];
    char password[30];
    double money;
    struct user *nextUser;
} User;

typedef struct bank {
    double totalMoney;
    int totalUsers;
} Bank;

void insertUser(User **startNodeUsers);
void deleteUser(User **startNodeUsers, char login[]);
int confirmPassword(User *user);
void clearTerminal(void);
void waitingEnter(void);

int main(void) {

    User *startNodeUser = NULL;
    

}

void insertUser(User **startNodeUsers) {

    User *currentNodeUser = NULL;
    User *previousNodeUser = NULL;
    
    User *newNodeUser = malloc(sizeof(User));

    if (newNodeUser == NULL) {
        printf(RED"\n\nFalha na alocacao dinamica. Encerrando...\n\n"RESET);
        exit(0);
    }

    currentNodeUser = *startNodeUsers;

    if (currentNodeUser == NULL) {
        *startNodeUsers = newNodeUser;
        newNodeUser->nextUser = NULL;
    }
    else {

        while (currentNodeUser != NULL) {
            previousNodeUser = currentNodeUser;
            currentNodeUser = currentNodeUser->nextUser;
        }

        currentNodeUser->nextUser = newNodeUser;
        newNodeUser->nextUser = NULL;
    }
}

void deleteUser(User **startNodeUsers, char login[]) {

    User *currentNodeUser = NULL;
    User *previousNodeUser = NULL;

    currentNodeUser= *startNodeUsers;

    while (currentNodeUser->login != login && currentNodeUser != NULL) {
        previousNodeUser = currentNodeUser;
        currentNodeUser = currentNodeUser->nextUser;
    }

    if (currentNodeUser == NULL) {
        printf(RED"\n\nO login informado nao existe.\n\n"RESET);
    }
    else {

        if (confirmPassword(currentNodeUser)) {
            previousNodeUser->nextUser = currentNodeUser->nextUser;
            free(currentNodeUser);

            printf(GREEN"\nA sua conta foi deletada.\n"RESET);
            waitingEnter();
        }
        else {
            printf(RED"\nVoce nao tem permissao para deletar esta conta.\n"RESET);
        }
    }
}

int confirmPassword(User *user) {

    char password[50];

    printf("Digite a senha para acessar a conta: ");
    scanf("%[^\n]", password);

    if (strcmp(user->password, password) == 0) {
        printf(GREEN"\n\nAcesso permitido.\n\n"RESET);
        return 1;
    }

    return 0;
}

void clearTerminal(void) {
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif
}

void waitingEnter(void) {
    printf(GREEN"Pressione enter para continuar: "RESET);
    getchar();
    getchar();
}