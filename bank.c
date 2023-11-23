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
    int ID;
    struct user *nextUser;
    struct user *previousUser;
} User;

typedef struct bank {
    double totalMoney;
    int totalUsers;
} Bank;

void insertUser(User **startNodeUsers);
void deleteUser(User **startNodeUsers, char login[]);
int confirmPassword(User *user);
void setAccountInformation(User **startNodeUsers, User *user);
int checkAvaliableLogin(User **startNodeUsers, char login[]);
User* accountLogin(User **startNodeUsers);
User* searchLoginUser(User **startNodeUsers, char login[]);
void startProgram(User **startNodeUsers);
void clearTerminal(void);
void waitingEnter(void);
void interfaceMenuOptions(void);
void interfaceMenuOptionsAccount(User **startNodeUsers, User *user);
void interfaceStartProgram(void);
int isNull(User *user);

int main(void) {

    User *startNodeUser = NULL;

    interfaceStartProgram();

    startProgram(&startNodeUser);
    

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
        newNodeUser->previousUser = NULL;
        newNodeUser->ID = 1;
    }
    else {

        while (currentNodeUser->nextUser != NULL) {
            previousNodeUser = currentNodeUser;
            currentNodeUser = currentNodeUser->nextUser;
        }

        currentNodeUser->nextUser = newNodeUser;
        newNodeUser->previousUser = currentNodeUser;
        newNodeUser->nextUser = NULL;
        newNodeUser->ID = newNodeUser->previousUser->ID + 1;
    }
    
    setAccountInformation(startNodeUsers, newNodeUser);
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
    scanf(" %[^\n]", password);

    if (strcmp(user->password, password) == 0) {
        return 1;
    }

    return 0;
}

void setAccountInformation(User **startNodeUsers,User *user) {

    char name[50];
    char login[50];
    char password[50];

    printf("Digite o seu nome: ");
    scanf(" %[^\n]", user->name);

    printf("Digite o seu usuario de login: ");
    scanf(" %[^\n]", login);

    while (checkAvaliableLogin(startNodeUsers, login) == 0) {
        printf(RED"\nLogin nao disponivel. Por favor, digite outro login\n"RESET);
        printf("Digite o seu usuario de login: ");
        scanf(" %[^\n]", login);
    }

    strcpy(user->login, login);

    printf("Digite a sua senha: ");
    scanf(" %[^\n]", user->password);

    user->money = 0.0;

    printf(GREEN"\n\nConta criada com sucesso!\n\n"RESET);
    waitingEnter();
}

int checkAvaliableLogin(User **startNodeUsers, char login[]) {

    User *currentUser = NULL;

    currentUser = *startNodeUsers;

    while (currentUser != NULL) {

        if (strcmp(currentUser->login, login) == 0) {
            return 0;
        }

        currentUser = currentUser->nextUser;
    }

    return 1;
}

void startProgram(User **startNodeUsers) {

    int escolha = 1;
    User *currentUser;

    while (escolha != 0) {

        interfaceMenuOptions();

        printf("Digite a sua escolha: ");
        scanf("%d", &escolha);

        switch(escolha) {
            case 1:
                currentUser = accountLogin(startNodeUsers);
                if (isNull(currentUser)) {
                    interfaceMenuOptionsAccount(startNodeUsers, currentUser);
                } 
                break;
            case 2:
                insertUser(startNodeUsers);
                break;
            case 0:
                printf(GREEN"\n\nEncerrando o programa...\n\n"RESET);
                break;
            default:
                printf(RED"\n\nEscolha invalida.\n\n"RESET);
                break;
        }
    }
}

User* searchLoginUser(User **startNodeUsers, char login[]) {

    User *currentUser = *startNodeUsers;

    while (currentUser != NULL) {

        if (strcmp(currentUser->login, login) == 0) {
            return currentUser;
        }

        currentUser = currentUser->nextUser;
    }

    return NULL;
}

User* accountLogin(User **startNodeUsers) {

    char login[30];
    char password[30];

    printf("Digite o seu usuario de login: ");
    scanf(" %[^\n]", login);

    User *currentUser = searchLoginUser(startNodeUsers, login);

    if (currentUser == NULL) {
        printf(RED"\n\nConta inexistente.\n\n"RESET);
        waitingEnter();
    }
    else {

        if (confirmPassword(currentUser)) {
            printf(GREEN"\n\nAcesso permitido.\n\n"RESET);
            waitingEnter();
            return currentUser;
        }
        else {
            printf(RED"\n\nAcesso negado.\n\n"RESET);
            waitingEnter();
            return NULL;
        }
    }

    return NULL;
}

int isNull(User *user) {
    if (user == NULL) {
        return 0;
    }
    
    return 1;
}

// ========================================== INTERFACE ============================================== //

void interfaceStartProgram(void) {

    clearTerminal();

    printf("$$$$$$$\\  $$\\   $$\\ $$\\       $$\\       $$$$$$$$\\ $$$$$$$$\\       $$$$$$$\\   $$$$$$\\  $$\\   $$\\ $$\\   $$\\ \n");
    printf("$$  __$$\\ $$ |  $$ |$$ |      $$ |      $$  _____|\\__$$  __|      $$  __$$\\ $$  __$$\\ $$$\\  $$ |$$ | $$  |\n");
    printf("$$ |  $$ |$$ |  $$ |$$ |      $$ |      $$ |         $$ |         $$ |  $$ |$$ /  $$ |$$$$\\ $$ |$$ |$$  / \n");
    printf("$$$$$$$\\ |$$ |  $$ |$$ |      $$ |      $$$$$\\       $$ |         $$$$$$$\\ |$$$$$$$$ |$$ $$\\$$ |$$$$$  /  \n");
    printf("$$  __$$\\ $$ |  $$ |$$ |      $$ |      $$  __|      $$ |         $$  __$$\\ $$  __$$ |$$ \\$$$$ |$$  $$<   \n");
    printf("$$ |  $$ |$$ |  $$ |$$ |      $$ |      $$ |         $$ |         $$ |  $$ |$$ |  $$ |$$ |\\$$$ |$$ |\\$$\\  \n");
    printf("$$$$$$$  |\\$$$$$$  |$$$$$$$$\\ $$$$$$$\\ $$$$$$$\\      $$ |         $$$$$$$  |$$ |  $$ |$$ | \\$$ |$$ | \\$$\\ \n");
    printf("\\_______/  \\______/ \\________|\\________|\\________|   \\__|         \\_______/ \\__|  \\__|\\__|  \\__|\\__|  \\__|\n");

    printf(GREEN"\n\nBem-vindo ao nosso banco!\n\n"RESET);
    waitingEnter();
}


void interfaceMenuOptions(void) {

    clearTerminal();

    printf("==============================================\n");
    printf("||              MENU DE OPCOES              ||\n");
    printf("==============================================\n");
    printf("|| [1] Login                                ||\n");
    printf("|| [2] Cadastrar                            ||\n");
    printf("|| [0] Sair                                 ||\n");
    printf("==============================================\n");
}

void interfaceMenuOptionsAccount(User **startNodeUsers, User *user) {

    clearTerminal();

    printf("==============================================\n");
    printf("||               MENU DE OPCOES             ||\n");
    printf("==============================================\n");
    printf("|| [1] Depositar                            ||\n");
    printf("|| [2] Sacar                                ||\n");
    printf("|| [3] Transferir                           ||\n");
    printf("|| [4] Consultar dados da conta             ||\n");
    printf("|| [5] Historico da conta                    ||\n");
    printf("|| [6] Excluir conta                        ||\n");
    printf("|| [0] Sair da conta                        ||\n");
    printf("==============================================\n");
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