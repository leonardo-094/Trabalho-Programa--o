#ifndef EQUIPAMENTOS_H
#define EQUIPAMENTOS_H

#include "utils.h"

typedef struct Equipamento {
    int codigo;
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    char marca[MAX_MARCA];
    char modelo[MAX_MODELO];
    char ip[MAX_IP];
    char mac[MAX_MAC];
    char localizacao[MAX_LOCALIZACAO];
    char estado[MAX_ESTADO];
    char ultimaVerificacao[MAX_DATA];
    struct Equipamento *next;
} Equipamento;

typedef struct EquipamentoData {
    int codigo;
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    char marca[MAX_MARCA];
    char modelo[MAX_MODELO];
    char ip[MAX_IP];
    char mac[MAX_MAC];
    char localizacao[MAX_LOCALIZACAO];
    char estado[MAX_ESTADO];
    char ultimaVerificacao[MAX_DATA];
} EquipamentoData;

Equipamento *criarEquipamento(int codigo);
void adicionarEquipamento(Equipamento **lista, Equipamento *novo);
void listarEquipamentos(const Equipamento *lista);
void equipamentos_carregar(Equipamento **lista, int *proximoCodigo);
void equipamentos_salvar(const Equipamento *lista);
void liberarEquipamentos(Equipamento **lista);

#endif // EQUIPAMENTOS_H
