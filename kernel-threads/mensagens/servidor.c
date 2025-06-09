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

    // Cria fila de mensagens
    msgid = msgget(KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    printf("Servidor esperando mensagem...\n");

    // Recebe mensagem do tipo 1
    if (msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }

    printf("Mensagem recebida: %s\n", msg.mtext);

    // Remove a fila após o uso
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
