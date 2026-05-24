#ifndef INCIDENTES_H
#define INCIDENTES_H

#include "utils.h"

typedef struct Incidente {
    int id;
    char origem[MAX_TIPO];
    int codigoEquipamento;
    char codigoSensor[MAX_NOME];
    char tipo[MAX_TIPO];
    char descricao[MAX_DESCRICAO];
    char prioridade[MAX_PRIORIDADE];
    char tecnico[MAX_TECNICO];
    char estado[MAX_ESTADO];
    char criado[MAX_DATA];
    char concluido[MAX_DATA];
    struct Incidente *next;
} Incidente;

typedef struct IncidenteData {
    int id;
    char origem[MAX_TIPO];
    int codigoEquipamento;
    char codigoSensor[MAX_NOME];
    char tipo[MAX_TIPO];
    char descricao[MAX_DESCRICAO];
    char prioridade[MAX_PRIORIDADE];
    char tecnico[MAX_TECNICO];
    char estado[MAX_ESTADO];
    char criado[MAX_DATA];
    char concluido[MAX_DATA];
} IncidenteData;

typedef struct FilaItem {
    Incidente *incidente;
    struct FilaItem *next;
} FilaItem;

Incidente *criarIncidente(int id, const char *origem, int codigoEquipamento, const char *codigoSensor,
                          const char *tipo, const char *descricao, const char *prioridade);
void adicionarIncidente(Incidente **lista, Incidente *novo);
void enfileirarIncidente(FilaItem **fila, Incidente *incidente);
Incidente *desenfileirarIncidente(FilaItem **fila);
void incidentes_carregar(Incidente **lista, int *proximoId);
void incidentes_salvar(const Incidente *lista);
void liberarIncidentes(Incidente **lista);
void liberarFila(FilaItem **fila);

#endif // INCIDENTES_H
