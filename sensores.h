#ifndef SENSORES_H
#define SENSORES_H

#include "utils.h"

typedef struct Incidente Incidente;
typedef struct FilaItem FilaItem;

typedef struct LeituraSensor {
    char codigo[MAX_NOME];
    char tipo[MAX_TIPO];
    float valor;
    char unidade[MAX_UNIDADE];
    char estado[MAX_ESTADO];
    struct LeituraSensor *next;
} LeituraSensor;

typedef struct LeituraSensorData {
    char codigo[MAX_NOME];
    char tipo[MAX_TIPO];
    float valor;
    char unidade[MAX_UNIDADE];
    char estado[MAX_ESTADO];
} LeituraSensorData;

void adicionarLeitura(LeituraSensor **lista, LeituraSensor *novo);
void importarSensores(LeituraSensor **leituras, Incidente **incidentes, FilaItem **fila, int *proximoId);
void listarLeituras(const LeituraSensor *lista);
LeituraSensor *pesquisarSensorPorCodigo(const LeituraSensor *lista, const char *codigo);
void listarAnomalias(const LeituraSensor *lista);
void sensores_carregar(LeituraSensor **lista);
void sensores_salvar(const LeituraSensor *lista);
void liberarLeituras(LeituraSensor **lista);

#endif // SENSORES_H
