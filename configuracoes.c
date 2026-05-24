#include "configuracoes.h"

static const char *CONFIGURACOES_FILE = "dados/configuracoes.dat";

static void appendConfiguracao(Configuracao **lista, Configuracao *novo) {
    if (!*lista) {
        *lista = novo;
    } else {
        Configuracao *atual = *lista;
        while (atual->next) {
            atual = atual->next;
        }
        atual->next = novo;
    }
}

void adicionarConfiguracao(Configuracao **lista, Configuracao *novo) {
    if (!novo) return;
    appendConfiguracao(lista, novo);
}

void empilharConfiguracao(PilhaItem **pilha, Configuracao *config) {
    if (!config) return;
    PilhaItem *item = malloc(sizeof(PilhaItem));
    if (!item) return;
    item->configuracao = config;
    item->next = *pilha;
    *pilha = item;
}

Configuracao *desempilharConfiguracao(PilhaItem **pilha) {
    if (!*pilha) return NULL;
    PilhaItem *topo = *pilha;
    Configuracao *config = topo->configuracao;
    *pilha = topo->next;
    free(topo);
    return config;
}

void configuracoes_carregar(Configuracao **lista) {
    FILE *f = fopen(CONFIGURACOES_FILE, "rb");
    if (!f) return;
    ConfiguracaoData data;
    while (fread(&data, sizeof(ConfiguracaoData), 1, f) == 1) {
        Configuracao *novo = malloc(sizeof(Configuracao));
        if (!novo) break;
        novo->codigoEquipamento = data.codigoEquipamento;
        strncpy(novo->tipo, data.tipo, MAX_TIPO);
        strncpy(novo->valorAnterior, data.valorAnterior, MAX_DESCRICAO);
        strncpy(novo->valorNovo, data.valorNovo, MAX_DESCRICAO);
        strncpy(novo->dataHora, data.dataHora, MAX_DATA);
        strncpy(novo->tecnico, data.tecnico, MAX_TECNICO);
        novo->next = NULL;
        appendConfiguracao(lista, novo);
    }
    fclose(f);
}

void configuracoes_salvar(const Configuracao *lista) {
    FILE *f = fopen(CONFIGURACOES_FILE, "wb");
    if (!f) return;
    const Configuracao *atual = lista;
    while (atual) {
        ConfiguracaoData data;
        data.codigoEquipamento = atual->codigoEquipamento;
        strncpy(data.tipo, atual->tipo, MAX_TIPO);
        strncpy(data.valorAnterior, atual->valorAnterior, MAX_DESCRICAO);
        strncpy(data.valorNovo, atual->valorNovo, MAX_DESCRICAO);
        strncpy(data.dataHora, atual->dataHora, MAX_DATA);
        strncpy(data.tecnico, atual->tecnico, MAX_TECNICO);
        fwrite(&data, sizeof(ConfiguracaoData), 1, f);
        atual = atual->next;
    }
    fclose(f);
}

void liberarConfiguracoes(Configuracao **lista) {
    Configuracao *atual = *lista;
    while (atual) {
        Configuracao *rem = atual;
        atual = atual->next;
        free(rem);
    }
    *lista = NULL;
}

void liberarPilha(PilhaItem **pilha) {
    while (*pilha) {
        PilhaItem *rem = *pilha;
        *pilha = rem->next;
        free(rem);
    }
}
