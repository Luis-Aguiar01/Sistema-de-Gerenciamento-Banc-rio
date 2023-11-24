#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define GOLDEN "\033[1;38;5;220m"

typedef struct user {
    char name[50];
    char login[30];
    char password[30];
    double money;
    int ID;
    char *history[30];
    int receiveTransfer;
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
void accountChoices(User **startNodeUsers , User *user);
void interfaceStartProgram(void);
int isNull(User *user);
void accountData(User *user);
void deposit(User *user);
void getMoney(User *user);
void transfer(User **startNodeUsers, User *user);
User *searchID(User **startNodeUsers, int ID);
void checkAccountData(User *user);
void printReceipt(User *user, User *account, double value);
int checkAvaliableValue(User *user, double value);
void showAccountDetails(User *user);

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
        waitingEnter();
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
            waitingEnter();
        }
    }
}

int confirmPassword(User *user) {

    char password[50];

    printf(BLUE"\nDigite a senha para acessar a conta: "RESET);
    scanf(" %[^\n]", password);

    if (strcmp(user->password, password) == 0) {
        return 1;
    }

    return 0;
}

void setAccountInformation(User **startNodeUsers, User *user) {

    clearTerminal();

    printf(GREEN"===============================================\n");
    printf("||             DADOS DA CONTA                ||\n");
    printf("===============================================\n"RESET);

    printf("\n\n");

    char name[50];
    char login[50];
    char password[50];

    printf(BLUE"Digite o seu nome: "RESET);
    scanf(" %[^\n]", user->name);

    printf("\n");

    printf(BLUE"Digite o seu usuario de login: "RESET);
    scanf(" %[^\n]", login);

    while (checkAvaliableLogin(startNodeUsers, login) == 0) {
        printf(RED"\nLogin nao disponivel. Por favor, digite outro login\n"RESET);
        printf(BLUE"Digite o seu usuario de login: "RESET);
        scanf(" %[^\n]", login);
    }

    printf("\n");

    strcpy(user->login, login);

    printf(BLUE"Digite a sua senha: "RESET);
    scanf(" %[^\n]", user->password);

    user->money = 0.0;
    user->receiveTransfer = 0;

    showAccountDetails(user);

    printf(GREEN"\n\nConta criada com sucesso!\n\n"RESET);
    waitingEnter();
}

void showAccountDetails(User *user) {

    clearTerminal();

    printf(GREEN"=====================================================================\n");
    printf("||                         DETALHES DA CONTA                       ||\n");
    printf("=====================================================================\n");
    printf("|| Nome: %-15s                                           ||\n", user->name);
    printf("|| Login: %-15s                                          ||\n", user->login);
    printf("|| ID: %-3d                                                         ||\n", user->ID);
    printf("|| Saldo: R$%-10.2lf                                             ||\n", user->money);
    printf("=====================================================================\n"RESET);

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

        printf(BLUE"\nDigite a sua escolha: "RESET);
        scanf("%d", &escolha);

        switch(escolha) {
            case 1:
                currentUser = accountLogin(startNodeUsers);
                if (isNull(currentUser)) {
                    accountChoices(startNodeUsers, currentUser);
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
                waitingEnter();
                break;
        }
    }
}

void accountChoices(User **startNodeUsers , User *user) {

    int choice = 1;

    while (choice != 0) {

        interfaceMenuOptionsAccount(startNodeUsers, user);

        printf(BLUE"\nDigite a sua escolha: "RESET);
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                deposit(user);
                break;
            case 2:
                getMoney(user);
                break;
            case 3:
                transfer(startNodeUsers, user);
                break;
            case 4:
                accountData(user);
                break;
            case 5:
                break;
            case 6:
                break;
            case 0:
                break;
            default:
                break;
        }
    }
}


User *searchLoginUser(User **startNodeUsers, char login[]) {

    User *currentUser = *startNodeUsers;

    while (currentUser != NULL) {

        if (strcmp(currentUser->login, login) == 0) {
            return currentUser;
        }

        currentUser = currentUser->nextUser;
    }

    return NULL;
}

User *accountLogin(User **startNodeUsers) {

    clearTerminal();

    printf(GREEN"===================================================\n");
    printf("||                AREA DE LOGIN                  ||\n");
    printf("===================================================\n"RESET);

    printf("\n\n");

    char login[30];
    char password[30];

    printf(BLUE"Digite o seu usuario de login: "RESET);
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

void deposit(User *user) {

    clearTerminal();

    double value;

    printf(GREEN"======================================================\n");
    printf("||                   AREA DE DEPOSITO               ||\n");
    printf("======================================================\n"RESET);

    printf("\n\n");

    printf(BLUE"Digite o valor do deposito: "RESET);
    scanf("%lf", &value);

    while (value < 0.0) {
        printf(RED"\n\nValor de deposito invalido. Digite um positivo.\n\n"RESET);
        printf(BLUE"Digite o valor do deposito: "RESET);
        scanf("%lf", &value);
    }

    user->money += value;

    printf(GREEN"\n\nDeposito de R$%.2lf realizado com sucesso!!\n\n"RESET, value);
    printf(GREEN"Saldo da conta: R$%.2lf\n", user->money);

    waitingEnter();
}

void getMoney(User *user) {

    clearTerminal();

    double value;

    printf(GREEN"================================================\n");
    printf("||               AREA DE SAQUE                ||\n");
    printf("================================================\n"RESET);

    printf("\n\n");

    printf(BLUE"Digite o valor do saque: "RESET);
    scanf("%lf", &value);

    while (value < 0.0) {
        printf(RED"\n\nValor invalido. Digite um valor positivo.\n\n"RESET);
        printf(BLUE"Digite o valor do saque: "RESET);
        scanf("%lf", &value);
    }

    if (user->money < value) {
        printf(RED"\n\nSaldo insuficiente na conta.\n\n"RESET);
    }
    else {
        user->money -= value;
        printf(GREEN"\n\nSaque de R$%.2lf realizado com sucesso!!\n\n", value);
        printf(GREEN"Saldo da conta: R$%.2lf\n", user->money);
    }

    waitingEnter();
}

void accountData(User *user) {

    clearTerminal();

    printf(GREEN"=========================================================\n");
    printf("||                     DADOS DA CONTA                  ||\n");
    printf("=========================================================\n");
    printf("|| Nome: %-15s                               ||\n", user->name);
    printf("|| Login: %-15s                              ||\n", user->login);
    printf("|| ID: %-3d                                             ||\n", user->ID);
    printf("|| Saldo: R$%-10.2f                                 ||\n", user->money);
    printf("=========================================================\n"RESET);

    waitingEnter();
}

int isNull(User *user) {
    if (user == NULL) {
        return 0;
    }
    
    return 1;
}

void transfer(User **startNodeUsers, User *user) {

    clearTerminal();

    printf(GREEN"========================================================\n");
    printf("||                 AREA DE TRANSFERENCIA              ||\n");
    printf("========================================================\n"RESET);

    printf("\n\n");

    int ID;
    char choice;
    double value;

    printf(BLUE"Digite o ID da conta a ser transferido o valor: "RESET);
    scanf("%d", &ID);

    User *accountForMoneyTransfer = searchID(startNodeUsers, ID);

    checkAccountData(accountForMoneyTransfer);

    printf(BLUE"\n\nDeseja transferir para essa conta[S/N]? "RESET);
    scanf(" %c", &choice);

    if (toupper(choice) == 'S') {
        
        printf(GREEN"\nSeu saldo atual: R$%.2lf\n\n"RESET, user->money);
        printf(BLUE"Digite o valor da transferencia: "RESET);
        scanf("%lf", &value);

        while ((value < 0.0) || (checkAvaliableValue(user, value) == 0)) {
            printf(RED"\n\nValor invalido. Digite um valor positivo e disponivel na conta.\n\n"RESET);
            printf(BLUE"Digite o valor da transferencia: "RESET);
            scanf("%lf", &value);
        }

        accountForMoneyTransfer->money += value;
        accountForMoneyTransfer->receiveTransfer = 1;
        user->money -= value;

        printReceipt(user, accountForMoneyTransfer, value);

        printf(GREEN"\n\nTransferencia realizada com sucesso!\n\n"RESET);
        printf(GREEN"Saldo atual: R$%.2lf\n"RESET, user->money);
        
        waitingEnter();
    }
    else {
        printf(RED"\n\nTransferencia cancelada.\n\n"RESET);
        waitingEnter();
    }

}

User *searchID(User **startNodeUsers, int ID) {

    User *currentUser = *startNodeUsers;

    while (currentUser != NULL && currentUser->ID != ID) {
        currentUser = currentUser->nextUser;
    }

    if(currentUser == NULL) {
        printf(RED"\n\nO ID informado nao existe na lista.\n\n"RESET);
        waitingEnter();
        return NULL;
    }
    
    printf(GREEN"\n\nID encontrado com sucesso!\n\n"RESET);

    return currentUser;
} 

void checkAccountData(User *user) {

    printf(GREEN"=========================================================\n");
    printf("||                     CONFIRMAR CONTA                 ||\n");
    printf("=========================================================\n");
    printf("|| Nome: %-15s                               ||\n", user->name);
    printf("|| ID: %-3d                                             ||\n", user->ID);
    printf("=========================================================\n"RESET);
}

void printReceipt(User *user, User *account, double value) {

    clearTerminal();

    printf(GREEN"======================================================================\n");
    printf("||                 DETALHES DA TRANSFERENCIA                        ||\n");
    printf("======================================================================\n");
    printf("|| Seu nome: %-15s                                        ||\n", user->name);
    printf("|| Enviado para: %-15s                                    ||\n", account->name);
    printf("|| Valor da transferencia: R$%-15.2lf                        ||\n", value);
    printf("======================================================================\n"RESET);
}

int checkAvaliableValue(User *user, double value) {

    if (user->money >= value) {
        return 1;
    }

    return 0;
}

// ========================================== INTERFACE ============================================== //

void interfaceStartProgram(void) {

    clearTerminal();

    printf(GREEN"$$$$$$$\\  $$\\   $$\\ $$\\       $$\\       $$$$$$$$\\ $$$$$$$$\\       $$$$$$$\\   $$$$$$\\  $$\\   $$\\ $$\\   $$\\ \n");
    printf("$$  __$$\\ $$ |  $$ |$$ |      $$ |      $$  _____|\\__$$  __|      $$  __$$\\ $$  __$$\\ $$$\\  $$ |$$ | $$  |\n");
    printf("$$ |  $$ |$$ |  $$ |$$ |      $$ |      $$ |         $$ |         $$ |  $$ |$$ /  $$ |$$$$\\ $$ |$$ |$$  / \n");
    printf("$$$$$$$\\ |$$ |  $$ |$$ |      $$ |      $$$$$\\       $$ |         $$$$$$$\\ |$$$$$$$$ |$$ $$\\$$ |$$$$$  /  \n");
    printf("$$  __$$\\ $$ |  $$ |$$ |      $$ |      $$  __|      $$ |         $$  __$$\\ $$  __$$ |$$ \\$$$$ |$$  $$<   \n");
    printf("$$ |  $$ |$$ |  $$ |$$ |      $$ |      $$ |         $$ |         $$ |  $$ |$$ |  $$ |$$ |\\$$$ |$$ |\\$$\\  \n");
    printf("$$$$$$$  |\\$$$$$$  |$$$$$$$$\\ $$$$$$$\\ $$$$$$$\\      $$ |         $$$$$$$  |$$ |  $$ |$$ | \\$$ |$$ | \\$$\\ \n");
    printf("\\_______/  \\______/ \\________|\\________|\\________|   \\__|         \\_______/ \\__|  \\__|\\__|  \\__|\\__|  \\__|\n"RESET);

    printf(GREEN"\n\nBem-vindo ao nosso banco!\n\n"RESET);
    waitingEnter();
}

void interfaceMenuOptions(void) {

    clearTerminal();

    printf(GREEN"==============================================\n");
    printf("||              MENU DE OPCOES              ||\n");
    printf("==============================================\n");
    printf("|| [1] Login                                ||\n");
    printf("|| [2] Cadastrar                            ||\n");
    printf("|| [0] Sair                                 ||\n");
    printf("==============================================\n"RESET);
}

void interfaceMenuOptionsAccount(User **startNodeUsers, User *user) {

    clearTerminal();

    printf(GREEN"==============================================\n");
    printf("||               MENU DE OPCOES             ||\n");
    printf("==============================================\n");
    printf("|| [1] Depositar                            ||\n");
    printf("|| [2] Sacar                                ||\n");
    printf("|| [3] Transferir                           ||\n");
    printf("|| [4] Consultar dados da conta             ||\n");
    printf("|| [5] Historico da conta                   ||\n");
    printf("|| [6] Excluir conta                        ||\n");
    printf("|| [0] Sair da conta                        ||\n");
    printf("==============================================\n"RESET);
}

void clearTerminal(void) {
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif
}

void waitingEnter(void) {
    printf(BLUE"\nPressione enter para continuar: "RESET);
    getchar();
    getchar();
}