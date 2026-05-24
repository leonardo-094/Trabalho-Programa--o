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
        strncpy(novo->tipo, data.tipo, MAX_TIPO);
        novo->valor = data.valor;
        strncpy(novo->unidade, data.unidade, MAX_UNIDADE);
        strncpy(novo->estado, data.estado, MAX_ESTADO);
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
        strncpy(data.tipo, atual->tipo, MAX_TIPO);
        data.valor = atual->valor;
        strncpy(data.unidade, atual->unidade, MAX_UNIDADE);
        strncpy(data.estado, atual->estado, MAX_ESTADO);
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
