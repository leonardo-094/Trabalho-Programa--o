#ifndef CONFIGURACOES_H
#define CONFIGURACOES_H

#include "utils.h"

// Estrutura que guarda uma alteração de configuração registada
typedef struct Configuracao {
    int codigoEquipamento;
    char tipo[MAX_TIPO];
    char valorAnterior[MAX_DESCRICAO];
    char valorNovo[MAX_DESCRICAO];
    char dataHora[MAX_DATA];
    char tecnico[MAX_TECNICO];
    struct Configuracao *next;
} Configuracao;

// Estrutura usada para gravar configurações no ficheiro binário
typedef struct ConfiguracaoData {
    int codigoEquipamento;
    char tipo[MAX_TIPO];
    char valorAnterior[MAX_DESCRICAO];
    char valorNovo[MAX_DESCRICAO];
    char dataHora[MAX_DATA];
    char tecnico[MAX_TECNICO];
} ConfiguracaoData;

// Item da pilha para guardar o histórico das últimas configurações
typedef struct PilhaItem {
    Configuracao *configuracao;
    struct PilhaItem *next;
} PilhaItem;

void adicionarConfiguracao(Configuracao **lista, Configuracao *novo);
void empilharConfiguracao(PilhaItem **pilha, Configuracao *config);
Configuracao *desempilharConfiguracao(PilhaItem **pilha);
void registarConfiguracao(Configuracao **lista, PilhaItem **pilha);
void reverterUltimaConfiguracao(PilhaItem **pilha);
void consultarUltimaConfiguracao(const PilhaItem *pilha);
void consultarNConfiguracoes(const Configuracao *lista, int n);
void consultarHistoricoEquipamento(const Configuracao *lista, int codigoEquipamento);
void limparConfiguracoes(Configuracao **lista, PilhaItem **pilha);
void configuracoes_carregar(Configuracao **lista, PilhaItem **pilha);
void configuracoes_salvar(const Configuracao *lista);
void liberarConfiguracoes(Configuracao **lista);
void liberarPilha(PilhaItem **pilha);

#endif // CONFIGURACOES_H
