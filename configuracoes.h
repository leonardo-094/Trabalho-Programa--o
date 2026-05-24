#ifndef CONFIGURACOES_H
#define CONFIGURACOES_H

#include "utils.h"

typedef struct Configuracao {
    int codigoEquipamento;
    char tipo[MAX_TIPO];
    char valorAnterior[MAX_DESCRICAO];
    char valorNovo[MAX_DESCRICAO];
    char dataHora[MAX_DATA];
    char tecnico[MAX_TECNICO];
    struct Configuracao *next;
} Configuracao;

typedef struct ConfiguracaoData {
    int codigoEquipamento;
    char tipo[MAX_TIPO];
    char valorAnterior[MAX_DESCRICAO];
    char valorNovo[MAX_DESCRICAO];
    char dataHora[MAX_DATA];
    char tecnico[MAX_TECNICO];
} ConfiguracaoData;

typedef struct PilhaItem {
    Configuracao *configuracao;
    struct PilhaItem *next;
} PilhaItem;

void adicionarConfiguracao(Configuracao **lista, Configuracao *novo);
void empilharConfiguracao(PilhaItem **pilha, Configuracao *config);
Configuracao *desempilharConfiguracao(PilhaItem **pilha);
void configuracoes_carregar(Configuracao **lista);
void configuracoes_salvar(const Configuracao *lista);
void liberarConfiguracoes(Configuracao **lista);
void liberarPilha(PilhaItem **pilha);

#endif // CONFIGURACOES_H
