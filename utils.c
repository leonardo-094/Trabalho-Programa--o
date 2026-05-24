#include "utils.h"

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}

int lerInteiro(const char *prompt) {
    int valor;
    printf("%s", prompt);
    while (scanf("%d", &valor) != 1) {
        limparBuffer();
        printf("Entrada inválida. %s", prompt);
    }
    limparBuffer();
    return valor;
}

void lerTexto(const char *prompt, char *destino, int tamanho) {
    printf("%s", prompt);
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    }
}

char *agoraFormato(char *buffer, int tamanho) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buffer, tamanho, "%Y-%m-%d %H:%M:%S", tm);
    return buffer;
}
