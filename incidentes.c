#include "incidentes.h"

static const char *INCIDENTES_FILE = "dados/incidentes.dat";

static void appendIncidente(Incidente **lista, Incidente *novo) {
    if (!*lista) {
        *lista = novo;
    } else {
        Incidente *atual = *lista;
        while (atual->next) {
            atual = atual->next;
        }
        atual->next = novo;
    }
}

Incidente *criarIncidente(int id, const char *origem, int codigoEquipamento, const char *codigoSensor,
                          const char *tipo, const char *descricao, const char *prioridade) {
    Incidente *novo = malloc(sizeof(Incidente));
    if (!novo) return NULL;
    novo->id = id;
    strncpy(novo->origem, origem, MAX_TIPO);
    novo->origem[MAX_TIPO - 1] = '\0';
    novo->codigoEquipamento = codigoEquipamento;
    strncpy(novo->codigoSensor, codigoSensor ? codigoSensor : "", MAX_NOME);
    novo->codigoSensor[MAX_NOME - 1] = '\0';
    strncpy(novo->tipo, tipo, MAX_TIPO);
    novo->tipo[MAX_TIPO - 1] = '\0';
    strncpy(novo->descricao, descricao, MAX_DESCRICAO);
    novo->descricao[MAX_DESCRICAO - 1] = '\0';
    strncpy(novo->prioridade, prioridade, MAX_PRIORIDADE);
    novo->prioridade[MAX_PRIORIDADE - 1] = '\0';
    strcpy(novo->tecnico, "Não atribuído");
    strcpy(novo->estado, "Pendente");
    agoraFormato(novo->criado, MAX_DATA);
    novo->concluido[0] = '\0';
    novo->next = NULL;
    return novo;
}

void adicionarIncidente(Incidente **lista, Incidente *novo) {
    if (!novo) return;
    appendIncidente(lista, novo);
}

void enfileirarIncidente(FilaItem **fila, Incidente *incidente) {
    if (!incidente) return;
    FilaItem *item = malloc(sizeof(FilaItem));
    if (!item) return;
    item->incidente = incidente;
    item->next = NULL;
    if (!*fila) {
        *fila = item;
    } else {
        FilaItem *atual = *fila;
        while (atual->next) {
            atual = atual->next;
        }
        atual->next = item;
    }
}

Incidente *desenfileirarIncidente(FilaItem **fila) {
    if (!*fila) return NULL;
    FilaItem *primeiro = *fila;
    Incidente *incidente = primeiro->incidente;
    *fila = primeiro->next;
    free(primeiro);
    return incidente;
}

void incidentes_carregar(Incidente **lista, int *proximoId) {
    FILE *f = fopen(INCIDENTES_FILE, "rb");
    if (!f) return;
    IncidenteData data;
    int maxId = *proximoId - 1;
    while (fread(&data, sizeof(IncidenteData), 1, f) == 1) {
        Incidente *novo = malloc(sizeof(Incidente));
        if (!novo) break;
        novo->id = data.id;
        strncpy(novo->origem, data.origem, MAX_TIPO);
        novo->origem[MAX_TIPO - 1] = '\0';
        novo->codigoEquipamento = data.codigoEquipamento;
        strncpy(novo->codigoSensor, data.codigoSensor, MAX_NOME);
        novo->codigoSensor[MAX_NOME - 1] = '\0';
        strncpy(novo->tipo, data.tipo, MAX_TIPO);
        novo->tipo[MAX_TIPO - 1] = '\0';
        strncpy(novo->descricao, data.descricao, MAX_DESCRICAO);
        novo->descricao[MAX_DESCRICAO - 1] = '\0';
        strncpy(novo->prioridade, data.prioridade, MAX_PRIORIDADE);
        novo->prioridade[MAX_PRIORIDADE - 1] = '\0';
        strncpy(novo->tecnico, data.tecnico, MAX_TECNICO);
        novo->tecnico[MAX_TECNICO - 1] = '\0';
        strncpy(novo->estado, data.estado, MAX_ESTADO);
        novo->estado[MAX_ESTADO - 1] = '\0';
        strncpy(novo->criado, data.criado, MAX_DATA);
        novo->criado[MAX_DATA - 1] = '\0';
        strncpy(novo->concluido, data.concluido, MAX_DATA);
        novo->concluido[MAX_DATA - 1] = '\0';
        novo->next = NULL;
        appendIncidente(lista, novo);
        if (novo->id > maxId) {
            maxId = novo->id;
        }
    }
    *proximoId = maxId + 1;
    fclose(f);
}

void incidentes_salvar(const Incidente *lista) {
    FILE *f = fopen(INCIDENTES_FILE, "wb");
    if (!f) return;
    const Incidente *atual = lista;
    while (atual) {
        IncidenteData data;
        data.id = atual->id;
        strncpy(data.origem, atual->origem, MAX_TIPO);
        data.origem[MAX_TIPO - 1] = '\0';
        data.codigoEquipamento = atual->codigoEquipamento;
        strncpy(data.codigoSensor, atual->codigoSensor, MAX_NOME);
        data.codigoSensor[MAX_NOME - 1] = '\0';
        strncpy(data.tipo, atual->tipo, MAX_TIPO);
        data.tipo[MAX_TIPO - 1] = '\0';
        strncpy(data.descricao, atual->descricao, MAX_DESCRICAO);
        data.descricao[MAX_DESCRICAO - 1] = '\0';
        strncpy(data.prioridade, atual->prioridade, MAX_PRIORIDADE);
        data.prioridade[MAX_PRIORIDADE - 1] = '\0';
        strncpy(data.tecnico, atual->tecnico, MAX_TECNICO);
        data.tecnico[MAX_TECNICO - 1] = '\0';
        strncpy(data.estado, atual->estado, MAX_ESTADO);
        data.estado[MAX_ESTADO - 1] = '\0';
        strncpy(data.criado, atual->criado, MAX_DATA);
        data.criado[MAX_DATA - 1] = '\0';
        strncpy(data.concluido, atual->concluido, MAX_DATA);
        data.concluido[MAX_DATA - 1] = '\0';
        fwrite(&data, sizeof(IncidenteData), 1, f);
        atual = atual->next;
    }
    fclose(f);
}

void liberarIncidentes(Incidente **lista) {
    Incidente *atual = *lista;
    while (atual) {
        Incidente *rem = atual;
        atual = atual->next;
        free(rem);
    }
    *lista = NULL;
}

void liberarFila(FilaItem **fila) {
    while (*fila) {
        FilaItem *rem = *fila;
        *fila = rem->next;
        free(rem);
    }
}
