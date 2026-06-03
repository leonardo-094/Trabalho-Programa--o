#ifndef INCIDENTES_H
#define INCIDENTES_H

#include "utils.h"

// Estrutura de um incidente registado
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

// Estrutura usada para guardar um incidente em ficheiro binário
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

// Item da fila de atendimento dos incidentes pendentes
typedef struct FilaItem {
    Incidente *incidente;
    struct FilaItem *next;
} FilaItem;

Incidente *criarIncidente(int id, const char *origem, int codigoEquipamento, const char *codigoSensor,
                          const char *tipo, const char *descricao, const char *prioridade);
Incidente *criarIncidenteManual(int id);
bool processarProximoIncidente(FilaItem **fila);
bool concluirIncidente(Incidente *lista, int id);
void listarIncidentesPorEstado(const Incidente *lista, const char *estado);
void listarIncidentesPorEquipamento(const Incidente *lista, int codigoEquipamento);
void listarIncidentesPorSensor(const Incidente *lista, const char *codigoSensor);
void listarIncidentesPorPrioridade(const Incidente *lista, const char *prioridade);
void adicionarIncidente(Incidente **lista, Incidente *novo);
void enfileirarIncidente(FilaItem **fila, Incidente *incidente);
Incidente *desenfileirarIncidente(FilaItem **fila);
void incidentes_carregar(Incidente **lista, int *proximoId, FilaItem **fila);
void incidentes_salvar(const Incidente *lista);
void liberarIncidentes(Incidente **lista);
void liberarFila(FilaItem **fila);

#endif // INCIDENTES_H
