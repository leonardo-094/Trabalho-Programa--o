#include "configuracoes.h"

static const char *CONFIGURACOES_FILE = "dados/configuracoes.dat";

static Configuracao *criarConfiguracao(int codigoEquipamento, const char *tipo, const char *valorAnterior,
                                         const char *valorNovo, const char *tecnico) {
    Configuracao *novo = malloc(sizeof(Configuracao));
    if (!novo) return NULL;
    novo->codigoEquipamento = codigoEquipamento;
    strncpy(novo->tipo, tipo, MAX_TIPO);
    novo->tipo[MAX_TIPO - 1] = '\0';
    strncpy(novo->valorAnterior, valorAnterior, MAX_DESCRICAO);
    novo->valorAnterior[MAX_DESCRICAO - 1] = '\0';
    strncpy(novo->valorNovo, valorNovo, MAX_DESCRICAO);
    novo->valorNovo[MAX_DESCRICAO - 1] = '\0';
    agoraFormato(novo->dataHora, MAX_DATA);
    strncpy(novo->tecnico, tecnico, MAX_TECNICO);
    novo->tecnico[MAX_TECNICO - 1] = '\0';
    novo->next = NULL;
    return novo;
}

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

void registarConfiguracao(Configuracao **lista, PilhaItem **pilha) {
    int codigoEquipamento = lerInteiro("Código do equipamento: ");
    char tipo[MAX_TIPO];
    char valorAnterior[MAX_DESCRICAO];
    char valorNovo[MAX_DESCRICAO];
    char tecnico[MAX_TECNICO];

    lerTexto("Tipo de configuração: ", tipo, MAX_TIPO);
    lerTexto("Valor anterior: ", valorAnterior, MAX_DESCRICAO);
    lerTexto("Novo valor: ", valorNovo, MAX_DESCRICAO);
    lerTexto("Técnico responsável: ", tecnico, MAX_TECNICO);

    Configuracao *config = criarConfiguracao(codigoEquipamento, tipo, valorAnterior, valorNovo, tecnico);
    if (!config) {
        printf("Não foi possível registar a configuração.\n");
        return;
    }
    adicionarConfiguracao(lista, config);
    empilharConfiguracao(pilha, config);
    printf("Configuração registada e empilhada com sucesso.\n");
}

void reverterUltimaConfiguracao(PilhaItem **pilha) {
    if (!pilha || !*pilha) {
        printf("Não existem configurações para reverter.\n");
        return;
    }

    Configuracao *config = desempilharConfiguracao(pilha);
    if (!config) {
        printf("Não foi possível desempilhar a configuração.\n");
        return;
    }

    printf("Configuração revertida:\n");
    printf("Equipamento: %d\n", config->codigoEquipamento);
    printf("Tipo: %s\n", config->tipo);
    printf("Valor anterior: %s\n", config->valorAnterior);
    printf("Valor revertido para: %s\n", config->valorAnterior);
    printf("Técnico: %s\n", config->tecnico);
    printf("Data/Hora: %s\n", config->dataHora);

    strncpy(config->valorNovo, config->valorAnterior, MAX_DESCRICAO);
    config->valorNovo[MAX_DESCRICAO - 1] = '\0';
    printf("Valor anterior restaurado no registo.\n");
}

void consultarUltimaConfiguracao(const PilhaItem *pilha) {
    if (!pilha) {
        printf("Nenhuma configuração na pilha.\n");
        return;
    }
    Configuracao *config = pilha->configuracao;
    printf("Última configuração na pilha:\n");
    printf("Equipamento: %d\n", config->codigoEquipamento);
    printf("Tipo: %s\n", config->tipo);
    printf("Valor anterior: %s\n", config->valorAnterior);
    printf("Valor novo: %s\n", config->valorNovo);
    printf("Técnico: %s\n", config->tecnico);
    printf("Data/Hora: %s\n", config->dataHora);
}

void consultarNConfiguracoes(const Configuracao *lista, int n) {
    if (!lista || n <= 0) {
        printf("Nenhuma configuração para mostrar.\n");
        return;
    }

    int total = 0;
    const Configuracao *atual = lista;
    while (atual) {
        total++;
        atual = atual->next;
    }

    int inicio = total - n;
    if (inicio < 0) inicio = 0;
    int indice = 0;
    atual = lista;
    while (atual) {
        if (indice >= inicio) {
            printf("Configuração %d:\n", indice + 1);
            printf("  Equipamento: %d\n", atual->codigoEquipamento);
            printf("  Tipo: %s\n", atual->tipo);
            printf("  Valor anterior: %s\n", atual->valorAnterior);
            printf("  Valor novo: %s\n", atual->valorNovo);
            printf("  Técnico: %s\n", atual->tecnico);
            printf("  Data/Hora: %s\n", atual->dataHora);
        }
        atual = atual->next;
        indice++;
    }
}

void consultarHistoricoEquipamento(const Configuracao *lista, int codigoEquipamento) {
    if (!lista) {
        printf("Nenhuma configuração registada.\n");
        return;
    }

    const Configuracao *atual = lista;
    int contador = 0;
    printf("Histórico de configurações para equipamento %d:\n", codigoEquipamento);
    while (atual) {
        if (atual->codigoEquipamento == codigoEquipamento) {
            printf("Configuração %d:\n", contador + 1);
            printf("  Tipo: %s\n", atual->tipo);
            printf("  Valor anterior: %s\n", atual->valorAnterior);
            printf("  Valor novo: %s\n", atual->valorNovo);
            printf("  Técnico: %s\n", atual->tecnico);
            printf("  Data/Hora: %s\n", atual->dataHora);
            contador++;
        }
        atual = atual->next;
    }
    if (contador == 0) {
        printf("Nenhuma configuração encontrada para o equipamento %d.\n", codigoEquipamento);
    }
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

void configuracoes_carregar(Configuracao **lista, PilhaItem **pilha) {
    FILE *f = fopen(CONFIGURACOES_FILE, "rb");
    if (!f) return;
    ConfiguracaoData data;
    while (fread(&data, sizeof(ConfiguracaoData), 1, f) == 1) {
        Configuracao *novo = malloc(sizeof(Configuracao));
        if (!novo) break;
        novo->codigoEquipamento = data.codigoEquipamento;
        strncpy(novo->tipo, data.tipo, MAX_TIPO);
        novo->tipo[MAX_TIPO - 1] = '\0';
        strncpy(novo->valorAnterior, data.valorAnterior, MAX_DESCRICAO);
        novo->valorAnterior[MAX_DESCRICAO - 1] = '\0';
        strncpy(novo->valorNovo, data.valorNovo, MAX_DESCRICAO);
        novo->valorNovo[MAX_DESCRICAO - 1] = '\0';
        strncpy(novo->dataHora, data.dataHora, MAX_DATA);
        novo->dataHora[MAX_DATA - 1] = '\0';
        strncpy(novo->tecnico, data.tecnico, MAX_TECNICO);
        novo->tecnico[MAX_TECNICO - 1] = '\0';
        novo->next = NULL;
        appendConfiguracao(lista, novo);
        if (pilha) {
            empilharConfiguracao(pilha, novo);
        }
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
        data.tipo[MAX_TIPO - 1] = '\0';
        strncpy(data.valorAnterior, atual->valorAnterior, MAX_DESCRICAO);
        data.valorAnterior[MAX_DESCRICAO - 1] = '\0';
        strncpy(data.valorNovo, atual->valorNovo, MAX_DESCRICAO);
        data.valorNovo[MAX_DESCRICAO - 1] = '\0';
        strncpy(data.dataHora, atual->dataHora, MAX_DATA);
        data.dataHora[MAX_DATA - 1] = '\0';
        strncpy(data.tecnico, atual->tecnico, MAX_TECNICO);
        data.tecnico[MAX_TECNICO - 1] = '\0';
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
