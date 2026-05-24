#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "equipamentos.h"
#include "incidentes.h"
#include "sensores.h"
#include "configuracoes.h"
#include "relatorios.h"

typedef struct Sistema {
    Equipamento *equipamentos;
    LeituraSensor *leituras;
    Incidente *incidentes;
    Configuracao *configuracoes;
    FilaItem *filaAtendimento;
    PilhaItem *pilhaConfiguracoes;
    int proximoCodigoEquipamento;
    int proximoIdIncidente;
} Sistema;

Sistema criarSistema(void) {
    Sistema sys = {0};
    sys.equipamentos = NULL;
    sys.leituras = NULL;
    sys.incidentes = NULL;
    sys.configuracoes = NULL;
    sys.filaAtendimento = NULL;
    sys.pilhaConfiguracoes = NULL;
    sys.proximoCodigoEquipamento = 1;
    sys.proximoIdIncidente = 1;
    return sys;
}

void carregarDados(Sistema *sys) {
    printf("Carregando dados automaticamente de ficheiros binários...\n");
    equipamentos_carregar(&sys->equipamentos, &sys->proximoCodigoEquipamento);
    incidentes_carregar(&sys->incidentes, &sys->proximoIdIncidente);
    configuracoes_carregar(&sys->configuracoes);
    sensores_carregar(&sys->leituras);
}

void guardarDados(Sistema *sys) {
    printf("Guardando dados em ficheiros binários...\n");
    equipamentos_salvar(sys->equipamentos);
    incidentes_salvar(sys->incidentes);
    configuracoes_salvar(sys->configuracoes);
    sensores_salvar(sys->leituras);
}

void liberarMemoria(Sistema *sys) {
    liberarEquipamentos(&sys->equipamentos);
    liberarLeituras(&sys->leituras);
    liberarIncidentes(&sys->incidentes);
    liberarConfiguracoes(&sys->configuracoes);
    liberarFila(&sys->filaAtendimento);
    liberarPilha(&sys->pilhaConfiguracoes);
}

void abrirMenuInventario(Sistema *sys) {
    int opcao;
    do {
        printf("\n=== Inventário de Equipamentos ===\n");
        printf("1. Adicionar equipamento\n");
        printf("2. Listar equipamentos\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1: {
                Equipamento *novo = criarEquipamento(sys->proximoCodigoEquipamento++);
                adicionarEquipamento(&sys->equipamentos, novo);
                break;
            }
            case 2:
                listarEquipamentos(sys->equipamentos);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void abrirMenuConectividade(Sistema *sys) {
    int opcao;
    do {
        printf("\n=== Testes de Conectividade ===\n");
        printf("1. Executar ping a um equipamento\n");
        printf("2. Executar ping a todos os equipamentos\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1:
                printf("Funcionalidade de ping ainda não implementada.\n");
                break;
            case 2:
                printf("Funcionalidade de ping geral ainda não implementada.\n");
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void abrirMenuSensores(Sistema *sys) {
    int opcao;
    do {
        printf("\n=== Monitorização de Sensores ===\n");
        printf("1. Importar leituras de sensores\n");
        printf("2. Listar leituras\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1:
                printf("Importação de sensores ainda não implementada.\n");
                break;
            case 2:
                printf("Listagem de leituras ainda não implementada.\n");
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void abrirMenuIncidentes(Sistema *sys) {
    int opcao;
    do {
        printf("\n=== Incidentes Técnicos ===\n");
        printf("1. Criar incidente manualmente\n");
        printf("2. Processar próximo incidente da fila\n");
        printf("3. Listar incidentes\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1:
                printf("Criação manual de incidente ainda não implementada.\n");
                break;
            case 2:
                printf("Processamento da fila ainda não implementado.\n");
                break;
            case 3:
                printf("Listagem de incidentes ainda não implementada.\n");
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void abrirMenuConfiguracoes(Sistema *sys) {
    int opcao;
    do {
        printf("\n=== Registo de Configurações ===\n");
        printf("1. Registar nova configuração\n");
        printf("2. Reverter última configuração\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1:
                printf("Registo de configuração ainda não implementado.\n");
                break;
            case 2:
                printf("Reversão de configuração ainda não implementada.\n");
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void abrirMenuRelatorios(Sistema *sys) {
    int opcao;
    do {
        printf("\n=== Relatórios Técnicos ===\n");
        printf("1. Gerar relatório de estado da rede\n");
        printf("2. Gerar relatório mensal de incidentes\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1:
                gerarRelatorioEstado(sys->equipamentos, sys->incidentes, sys->leituras);
                break;
            case 2:
                gerarRelatorioMensal(sys->incidentes);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

int main(void) {
    Sistema sistema = criarSistema();
    carregarDados(&sistema);
    int opcao;
    do {
        printf("\n=== Mini NOC - Menu Principal ===\n");
        printf("1. Inventário de Equipamentos\n");
        printf("2. Testes de Conectividade\n");
        printf("3. Monitorização de Sensores\n");
        printf("4. Incidentes Técnicos\n");
        printf("5. Registo de Configurações\n");
        printf("6. Relatórios\n");
        printf("7. Carregar dados\n");
        printf("8. Guardar dados\n");
        printf("0. Sair\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1:
                abrirMenuInventario(&sistema);
                break;
            case 2:
                abrirMenuConectividade(&sistema);
                break;
            case 3:
                abrirMenuSensores(&sistema);
                break;
            case 4:
                abrirMenuIncidentes(&sistema);
                break;
            case 5:
                abrirMenuConfiguracoes(&sistema);
                break;
            case 6:
                abrirMenuRelatorios(&sistema);
                break;
            case 7:
                carregarDados(&sistema);
                break;
            case 8:
                guardarDados(&sistema);
                break;
            case 0:
                printf("A sair... Guardando dados antes de fechar.\n");
                guardarDados(&sistema);
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    liberarMemoria(&sistema);
    return 0;
}
