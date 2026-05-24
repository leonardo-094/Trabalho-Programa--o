#include "sensores.h"

static const char *LEITURAS_FILE = "dados/leituras_sensores.dat";

static void appendLeitura(LeituraSensor **lista, LeituraSensor *novo) {
    if (!*lista) {
        *lista = novo;
    } else {
        LeituraSensor *atual = *lista;
        while (atual->next) {
            atual = atual->next;
        }
        atual->next = novo;
    }
}

void adicionarLeitura(LeituraSensor **lista, LeituraSensor *novo) {
    if (!novo) return;
    appendLeitura(lista, novo);
}

void sensores_carregar(LeituraSensor **lista) {
    FILE *f = fopen(LEITURAS_FILE, "rb");
    if (!f) return;
    LeituraSensorData data;
    while (fread(&data, sizeof(LeituraSensorData), 1, f) == 1) {
        LeituraSensor *novo = malloc(sizeof(LeituraSensor));
        if (!novo) break;
        strncpy(novo->codigo, data.codigo, MAX_NOME);
        novo->codigo[MAX_NOME - 1] = '\0';
        strncpy(novo->tipo, data.tipo, MAX_TIPO);
        novo->tipo[MAX_TIPO - 1] = '\0';
        novo->valor = data.valor;
        strncpy(novo->unidade, data.unidade, MAX_UNIDADE);
        novo->unidade[MAX_UNIDADE - 1] = '\0';
        strncpy(novo->estado, data.estado, MAX_ESTADO);
        novo->estado[MAX_ESTADO - 1] = '\0';
        novo->next = NULL;
        appendLeitura(lista, novo);
    }
    fclose(f);
}

void sensores_salvar(const LeituraSensor *lista) {
    FILE *f = fopen(LEITURAS_FILE, "wb");
    if (!f) return;
    const LeituraSensor *atual = lista;
    while (atual) {
        LeituraSensorData data;
        strncpy(data.codigo, atual->codigo, MAX_NOME);
        data.codigo[MAX_NOME - 1] = '\0';
        strncpy(data.tipo, atual->tipo, MAX_TIPO);
        data.tipo[MAX_TIPO - 1] = '\0';
        data.valor = atual->valor;
        strncpy(data.unidade, atual->unidade, MAX_UNIDADE);
        data.unidade[MAX_UNIDADE - 1] = '\0';
        strncpy(data.estado, atual->estado, MAX_ESTADO);
        data.estado[MAX_ESTADO - 1] = '\0';
        fwrite(&data, sizeof(LeituraSensorData), 1, f);
        atual = atual->next;
    }
    fclose(f);
}

void liberarLeituras(LeituraSensor **lista) {
    LeituraSensor *atual = *lista;
    while (atual) {
        LeituraSensor *rem = atual;
        atual = atual->next;
        free(rem);
    }
    *lista = NULL;
}
