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

bool processarProximoIncidente(FilaItem **fila) {
    Incidente *inc = desenfileirarIncidente(fila);
    if (!inc) {
        printf("Nenhum incidente na fila de atendimento.\n");
        return false;
    }
    strcpy(inc->estado, "Em Curso");
    printf("Incidente %d em curso:\n", inc->id);
    printf("Origem: %s\n", inc->origem);
    printf("Código do equipamento: %d\n", inc->codigoEquipamento);
    printf("Código do sensor: %s\n", inc->codigoSensor);
    printf("Tipo: %s\n", inc->tipo);
    printf("Descrição: %s\n", inc->descricao);
    printf("Prioridade: %s\n", inc->prioridade);
    printf("Estado: %s\n", inc->estado);
    printf("Criado: %s\n", inc->criado);
    return true;
}

bool concluirIncidente(Incidente *lista, int id) {
    Incidente *atual = lista;
    while (atual) {
        if (atual->id == id) {
            strcpy(atual->estado, "Concluido");
            agoraFormato(atual->concluido, MAX_DATA);
            printf("Incidente %d concluído em %s.\n", id, atual->concluido);
            return true;
        }
        atual = atual->next;
    }
    printf("Incidente %d não encontrado.\n", id);
    return false;
}

void listarIncidentesPorEstado(const Incidente *lista, const char *estado) {
    const Incidente *atual = lista;
    int contador = 0;
    printf("\nIncidentes com estado %s:\n", estado);
    printf("------------------------------------------------------------\n");
    while (atual) {
        if (strcmp(atual->estado, estado) == 0) {
            printf("ID: %d | Equipamento: %d | Sensor: %s | Tipo: %s | Prioridade: %s | Estado: %s\n",
                   atual->id, atual->codigoEquipamento, atual->codigoSensor, atual->tipo, atual->prioridade, atual->estado);
            contador++;
        }
        atual = atual->next;
    }
    if (contador == 0) {
        printf("Nenhum incidente encontrado com estado %s.\n", estado);
    }
    printf("------------------------------------------------------------\n");
}

void listarIncidentesPorEquipamento(const Incidente *lista, int codigoEquipamento) {
    const Incidente *atual = lista;
    int contador = 0;
    printf("\nIncidentes do equipamento %d:\n", codigoEquipamento);
    printf("------------------------------------------------------------\n");
    while (atual) {
        if (atual->codigoEquipamento == codigoEquipamento) {
            printf("ID: %d | Sensor: %s | Tipo: %s | Prioridade: %s | Estado: %s\n",
                   atual->id, atual->codigoSensor, atual->tipo, atual->prioridade, atual->estado);
            contador++;
        }
        atual = atual->next;
    }
    if (contador == 0) {
        printf("Nenhum incidente encontrado para o equipamento %d.\n", codigoEquipamento);
    }
    printf("------------------------------------------------------------\n");
}

void listarIncidentesPorSensor(const Incidente *lista, const char *codigoSensor) {
    const Incidente *atual = lista;
    int contador = 0;
    printf("\nIncidentes do sensor %s:\n", codigoSensor);
    printf("------------------------------------------------------------\n");
    while (atual) {
        if (strcmp(atual->codigoSensor, codigoSensor) == 0) {
            printf("ID: %d | Equipamento: %d | Tipo: %s | Prioridade: %s | Estado: %s\n",
                   atual->id, atual->codigoEquipamento, atual->tipo, atual->prioridade, atual->estado);
            contador++;
        }
        atual = atual->next;
    }
    if (contador == 0) {
        printf("Nenhum incidente encontrado para o sensor %s.\n", codigoSensor);
    }
    printf("------------------------------------------------------------\n");
}

void listarIncidentesPorPrioridade(const Incidente *lista, const char *prioridade) {
    const Incidente *atual = lista;
    int contador = 0;
    printf("\nIncidentes com prioridade %s:\n", prioridade);
    printf("------------------------------------------------------------\n");
    while (atual) {
        if (strcmp(atual->prioridade, prioridade) == 0) {
            printf("ID: %d | Equipamento: %d | Sensor: %s | Tipo: %s | Estado: %s\n",
                   atual->id, atual->codigoEquipamento, atual->codigoSensor, atual->tipo, atual->estado);
            contador++;
        }
        atual = atual->next;
    }
    if (contador == 0) {
        printf("Nenhum incidente encontrado com prioridade %s.\n", prioridade);
    }
    printf("------------------------------------------------------------\n");
}

Incidente *criarIncidenteManual(int id) {
    char origem[MAX_TIPO];
    char codigoSensor[MAX_NOME];
    char tipo[MAX_TIPO];
    char descricao[MAX_DESCRICAO];
    char prioridade[MAX_PRIORIDADE];
    int codigoEquipamento = lerInteiro("Código do equipamento associado: ");
    lerTexto("Origem do incidente: ", origem, MAX_TIPO);
    lerTexto("Código do sensor (se aplicável): ", codigoSensor, MAX_NOME);
    lerTexto("Tipo do incidente: ", tipo, MAX_TIPO);
    lerTexto("Descrição do incidente: ", descricao, MAX_DESCRICAO);
    lerTexto("Prioridade (Alta/Média/Baixa): ", prioridade, MAX_PRIORIDADE);

    return criarIncidente(id, origem, codigoEquipamento, codigoSensor, tipo, descricao, prioridade);
}

void incidentes_carregar(Incidente **lista, int *proximoId, FilaItem **fila) {
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
        if (strcmp(novo->estado, "Pendente") == 0 && fila) {
            enfileirarIncidente(fila, novo);
        }
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
