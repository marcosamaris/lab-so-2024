#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define KEY 1234

struct mensagem {
    long mtype;
    char mtext[100];
};

int main() {
    int msgid;
    struct mensagem msg;

    // Cria ou obtém fila de mensagens
    msgid = msgget(KEY, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Digite a mensagem a enviar: ");
    fgets(msg.mtext, sizeof(msg.mtext), stdin);
    msg.mtype = 1;

    if (msgsnd(msgid, &msg, strlen(msg.mtext)+1, 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    printf("Mensagem enviada.\n");
    return 0;
}
