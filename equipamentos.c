#include "equipamentos.h"

static const char *EQUIPAMENTOS_FILE = "dados/equipamentos.dat";

static void appendEquipamento(Equipamento **lista, Equipamento *novo) {
    if (!*lista) {
        *lista = novo;
    } else {
        Equipamento *atual = *lista;
        while (atual->next) {
            atual = atual->next;
        }
        atual->next = novo;
    }
}

Equipamento *criarEquipamento(int codigo) {
    Equipamento *novo = malloc(sizeof(Equipamento));
    if (!novo) return NULL;
    novo->codigo = codigo;
    lerTexto("Nome do equipamento: ", novo->nome, MAX_NOME);
    lerTexto("Tipo do equipamento: ", novo->tipo, MAX_TIPO);
    lerTexto("Marca: ", novo->marca, MAX_MARCA);
    lerTexto("Modelo: ", novo->modelo, MAX_MODELO);
    lerTexto("Endereço IP: ", novo->ip, MAX_IP);
    lerTexto("Endereço MAC: ", novo->mac, MAX_MAC);
    lerTexto("Localização física: ", novo->localizacao, MAX_LOCALIZACAO);
    strcpy(novo->estado, "Operacional");
    agoraFormato(novo->ultimaVerificacao, MAX_DATA);
    novo->next = NULL;
    return novo;
}

void adicionarEquipamento(Equipamento **lista, Equipamento *novo) {
    if (!novo) return;
    appendEquipamento(lista, novo);
    printf("Equipamento %s (%d) adicionado com sucesso.\n", novo->nome, novo->codigo);
}

void listarEquipamentos(const Equipamento *lista) {
    if (!lista) {
        printf("Nenhum equipamento registado.\n");
        return;
    }
    printf("\nEquipamentos registados:\n");
    printf("------------------------------------------------------------\n");
    const Equipamento *atual = lista;
    while (atual) {
        printf("Código: %d | Nome: %s | Tipo: %s | IP: %s | Estado: %s\n",
               atual->codigo, atual->nome, atual->tipo, atual->ip, atual->estado);
        atual = atual->next;
    }
    printf("------------------------------------------------------------\n");
}

void equipamentos_carregar(Equipamento **lista, int *proximoCodigo) {
    FILE *f = fopen(EQUIPAMENTOS_FILE, "rb");
    if (!f) return;
    EquipamentoData data;
    int maxCodigo = *proximoCodigo - 1;
    while (fread(&data, sizeof(EquipamentoData), 1, f) == 1) {
        Equipamento *novo = malloc(sizeof(Equipamento));
        if (!novo) break;
        novo->codigo = data.codigo;
        strncpy(novo->nome, data.nome, MAX_NOME);
        strncpy(novo->tipo, data.tipo, MAX_TIPO);
        strncpy(novo->marca, data.marca, MAX_MARCA);
        strncpy(novo->modelo, data.modelo, MAX_MODELO);
        strncpy(novo->ip, data.ip, MAX_IP);
        strncpy(novo->mac, data.mac, MAX_MAC);
        strncpy(novo->localizacao, data.localizacao, MAX_LOCALIZACAO);
        strncpy(novo->estado, data.estado, MAX_ESTADO);
        strncpy(novo->ultimaVerificacao, data.ultimaVerificacao, MAX_DATA);
        novo->next = NULL;
        appendEquipamento(lista, novo);
        if (novo->codigo > maxCodigo) {
            maxCodigo = novo->codigo;
        }
    }
    *proximoCodigo = maxCodigo + 1;
    fclose(f);
}

void equipamentos_salvar(const Equipamento *lista) {
    FILE *f = fopen(EQUIPAMENTOS_FILE, "wb");
    if (!f) return;
    const Equipamento *atual = lista;
    while (atual) {
        EquipamentoData data;
        data.codigo = atual->codigo;
        strncpy(data.nome, atual->nome, MAX_NOME);
        strncpy(data.tipo, atual->tipo, MAX_TIPO);
        strncpy(data.marca, atual->marca, MAX_MARCA);
        strncpy(data.modelo, atual->modelo, MAX_MODELO);
        strncpy(data.ip, atual->ip, MAX_IP);
        strncpy(data.mac, atual->mac, MAX_MAC);
        strncpy(data.localizacao, atual->localizacao, MAX_LOCALIZACAO);
        strncpy(data.estado, atual->estado, MAX_ESTADO);
        strncpy(data.ultimaVerificacao, atual->ultimaVerificacao, MAX_DATA);
        fwrite(&data, sizeof(EquipamentoData), 1, f);
        atual = atual->next;
    }
    fclose(f);
}

void liberarEquipamentos(Equipamento **lista) {
    Equipamento *atual = *lista;
    while (atual) {
        Equipamento *rem = atual;
        atual = atual->next;
        free(rem);
    }
    *lista = NULL;
}
