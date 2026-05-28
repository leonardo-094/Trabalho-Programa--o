#ifndef EQUIPAMENTOS_H
#define EQUIPAMENTOS_H

#include "utils.h"

typedef struct Incidente Incidente;

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
bool removerEquipamento(Equipamento **lista, const Incidente *incidentes, int codigo);
bool alterarEquipamento(Equipamento *lista, int codigo);
bool alterarEstadoEquipamento(Equipamento *lista, int codigo, const char *novoEstado);
Equipamento *pesquisarPorCodigo(const Equipamento *lista, int codigo);
Equipamento *pesquisarPorIP(const Equipamento *lista, const char *ip);
Equipamento *pesquisarPorMAC(const Equipamento *lista, const char *mac);
void listarPorTipo(const Equipamento *lista, const char *tipo);
void listarPorEstado(const Equipamento *lista, const char *estado);
void equipamentos_carregar(Equipamento **lista, int *proximoCodigo);
void equipamentos_salvar(const Equipamento *lista);
void liberarEquipamentos(Equipamento **lista);

#endif // EQUIPAMENTOS_H
