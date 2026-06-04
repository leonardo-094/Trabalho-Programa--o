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
    incidentes_carregar(&sys->incidentes, &sys->proximoIdIncidente, &sys->filaAtendimento);
    configuracoes_carregar(&sys->configuracoes, &sys->pilhaConfiguracoes);
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
        printf("3. Remover equipamento\n");
        printf("4. Alterar dados do equipamento\n");
        printf("5. Alterar estado do equipamento\n");
        printf("6. Pesquisar equipamento por código\n");
        printf("7. Pesquisar equipamento por IP\n");
        printf("8. Pesquisar equipamento por MAC\n");
        printf("9. Listar equipamentos por tipo\n");
        printf("10. Listar equipamentos por estado\n");
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
            case 3: {
                int codigo = lerInteiro("Código do equipamento a remover: ");
                removerEquipamento(&sys->equipamentos, sys->incidentes, codigo);
                break;
            }
            case 4: {
                int codigo = lerInteiro("Código do equipamento a alterar: ");
                alterarEquipamento(sys->equipamentos, codigo);
                break;
            }
            case 5: {
                int codigo = lerInteiro("Código do equipamento para mudar estado: ");
                char novoEstado[MAX_ESTADO];
                lerTexto("Novo estado (Operacional/Em Falha/Em Manutenção/Desativado): ", novoEstado, MAX_ESTADO);
                if (alterarEstadoEquipamento(sys->equipamentos, codigo, novoEstado)) {
                    printf("Estado do equipamento %d alterado com sucesso.\n", codigo);
                } else {
                    printf("Não foi possível alterar o estado do equipamento %d.\n", codigo);
                }
                break;
            }
            case 6: {
                int codigo = lerInteiro("Código do equipamento a pesquisar: ");
                Equipamento *eq = pesquisarPorCodigo(sys->equipamentos, codigo);
                if (eq) {
                    printf("Equipamento encontrado: Código %d | Nome: %s | Tipo: %s | IP: %s | Estado: %s\n",
                           eq->codigo, eq->nome, eq->tipo, eq->ip, eq->estado);
                } else {
                    printf("Equipamento %d não encontrado.\n", codigo);
                }
                break;
            }
            case 7: {
                char ip[MAX_IP];
                lerTexto("Endereço IP a pesquisar: ", ip, MAX_IP);
                Equipamento *eq = pesquisarPorIP(sys->equipamentos, ip);
                if (eq) {
                    printf("Equipamento encontrado: Código %d | Nome: %s | Tipo: %s | IP: %s | Estado: %s\n",
                           eq->codigo, eq->nome, eq->tipo, eq->ip, eq->estado);
                } else {
                    printf("Nenhum equipamento com IP %s encontrado.\n", ip);
                }
                break;
            }
            case 8: {
                char mac[MAX_MAC];
                lerTexto("Endereço MAC a pesquisar: ", mac, MAX_MAC);
                Equipamento *eq = pesquisarPorMAC(sys->equipamentos, mac);
                if (eq) {
                    printf("Equipamento encontrado: Código %d | Nome: %s | Tipo: %s | IP: %s | Estado: %s\n",
                           eq->codigo, eq->nome, eq->tipo, eq->ip, eq->estado);
                } else {
                    printf("Nenhum equipamento com MAC %s encontrado.\n", mac);
                }
                break;
            }
            case 9: {
                char tipo[MAX_TIPO];
                lerTexto("Tipo a listar: ", tipo, MAX_TIPO);
                listarPorTipo(sys->equipamentos, tipo);
                break;
            }
            case 10: {
                char estado[MAX_ESTADO];
                lerTexto("Estado a listar: ", estado, MAX_ESTADO);
                listarPorEstado(sys->equipamentos, estado);
                break;
            }
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

static bool executarPingDetectarResposta(const char *ficheiroResultado) {
    FILE *f = fopen(ficheiroResultado, "r");
    if (!f) {
        return false;
    }

    char linha[256];
    bool respondeu = false;
    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, "TTL=") || strstr(linha, "ttl=") || strstr(linha, "bytes from")) {
            respondeu = true;
            break;
        }
    }
    fclose(f);
    return respondeu;
}

static void registarLogMonitorizacao(const Equipamento *eq, const char *mensagem, bool respondeu) {
    char dataHora[MAX_DATA];
    agoraFormato(dataHora, MAX_DATA);
    FILE *log = fopen("dados/log_monitorizacao.txt", "a");
    if (!log) {
        return;
    }
    fprintf(log, "[%s] Equipamento %d (%s) IP %s - %s - %s\n",
            dataHora, eq->codigo, eq->nome, eq->ip, mensagem, respondeu ? "Respondeu" : "Não respondeu");
    fclose(log);
}

static void executarPingEquipamento(Sistema *sys, Equipamento *eq) {
    if (!eq) {
        printf("Equipamento inválido.\n");
        return;
    }

    char comando[256];
#ifdef _WIN32
    snprintf(comando, sizeof(comando), "ping -n 4 %s > dados/resultado_ping.txt 2>&1", eq->ip);
#else
    snprintf(comando, sizeof(comando), "ping -c 4 %s > dados/resultado_ping.txt 2>&1", eq->ip);
#endif
    system(comando);

    bool respondeu = executarPingDetectarResposta("dados/resultado_ping.txt");
    agoraFormato(eq->ultimaVerificacao, MAX_DATA);

    if (!respondeu) {
        strcpy(eq->estado, "Em Falha");

        char descricao[MAX_DESCRICAO];
        snprintf(descricao, sizeof(descricao), "Falha de ping para equipamento %d (%s).", eq->codigo, eq->nome);
        Incidente *inc = criarIncidente(sys->proximoIdIncidente++, "Monitorizacao", eq->codigo, "", "Conectividade", descricao, "Alta");
        if (inc) {
            adicionarIncidente(&sys->incidentes, inc);
            enfileirarIncidente(&sys->filaAtendimento, inc);
        }
    }

    registarLogMonitorizacao(eq, respondeu ? "Teste de ping concluído" : "Teste de ping falhou", respondeu);
    printf("Resultado do ping para %s (%s): %s\n", eq->nome, eq->ip, respondeu ? "Respondeu" : "Não respondeu");
    printf("Saída gravada em dados/resultado_ping.txt\n");
}

static void executarPingTodosEquipamentos(Sistema *sys) {
    if (!sys->equipamentos) {
        printf("Nenhum equipamento registado para testar.\n");
        return;
    }

    Equipamento *atual = sys->equipamentos;
    while (atual) {
        printf("\nExecutando ping para equipamento %d (%s)...\n", atual->codigo, atual->nome);
        executarPingEquipamento(sys, atual);
        atual = atual->next;
    }
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
            case 1: {
                if (!sys->equipamentos) {
                    printf("Nenhum equipamento registado.\n");
                    break;
                }
                listarEquipamentos(sys->equipamentos);
                int codigo = lerInteiro("Escolha o código do equipamento para ping: ");
                Equipamento *eq = pesquisarPorCodigo(sys->equipamentos, codigo);
                if (!eq) {
                    printf("Equipamento %d não encontrado.\n", codigo);
                } else {
                    executarPingEquipamento(sys, eq);
                }
                break;
            }
            case 2:
                executarPingTodosEquipamentos(sys);
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
        printf("3. Pesquisar sensor por código\n");
        printf("4. Listar anomalias\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1:
                importarSensores(&sys->leituras, &sys->incidentes, &sys->filaAtendimento, &sys->proximoIdIncidente);
                break;
            case 2:
                listarLeituras(sys->leituras);
                break;
            case 3: {
                char codigo[MAX_NOME];
                lerTexto("Código do sensor a pesquisar: ", codigo, MAX_NOME);
                LeituraSensor *leitura = pesquisarSensorPorCodigo(sys->leituras, codigo);
                if (leitura) {
                    printf("Sensor encontrado: Código: %s | Tipo: %s | Valor: %.2f %s | Estado: %s\n",
                           leitura->codigo, leitura->tipo, leitura->valor, leitura->unidade, leitura->estado);
                } else {
                    printf("Nenhum sensor com código %s encontrado.\n", codigo);
                }
                break;
            }
            case 4:
                listarAnomalias(sys->leituras);
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
        printf("3. Concluir incidente\n");
        printf("4. Listar incidentes por estado\n");
        printf("5. Listar incidentes por equipamento\n");
        printf("6. Listar incidentes por sensor\n");
        printf("7. Listar incidentes por prioridade\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1: {
                Incidente *inc = criarIncidenteManual(sys->proximoIdIncidente++);
                if (inc) {
                    adicionarIncidente(&sys->incidentes, inc);
                    enfileirarIncidente(&sys->filaAtendimento, inc);
                    printf("Incidente %d criado e adicionado à fila de atendimento.\n", inc->id);
                }
                break;
            }
            case 2:
                processarProximoIncidente(&sys->filaAtendimento);
                break;
            case 3: {
                int id = lerInteiro("ID do incidente a concluir: ");
                concluirIncidente(sys->incidentes, id);
                break;
            }
            case 4: {
                char estado[MAX_ESTADO];
                lerTexto("Estado a filtrar: ", estado, MAX_ESTADO);
                listarIncidentesPorEstado(sys->incidentes, estado);
                break;
            }
            case 5: {
                int codigo = lerInteiro("Código do equipamento a filtrar: ");
                listarIncidentesPorEquipamento(sys->incidentes, codigo);
                break;
            }
            case 6: {
                char codigoSensor[MAX_NOME];
                lerTexto("Código do sensor a filtrar: ", codigoSensor, MAX_NOME);
                listarIncidentesPorSensor(sys->incidentes, codigoSensor);
                break;
            }
            case 7: {
                char prioridade[MAX_PRIORIDADE];
                lerTexto("Prioridade a filtrar: ", prioridade, MAX_PRIORIDADE);
                listarIncidentesPorPrioridade(sys->incidentes, prioridade);
                break;
            }
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
        printf("3. Consultar última configuração\n");
        printf("4. Consultar N últimas configurações\n");
        printf("5. Consultar histórico de equipamento\n");
        printf("6. Limpar histórico de configurações\n");
        printf("0. Voltar\n");
        opcao = lerInteiro("Escolha uma opção: ");
        switch (opcao) {
            case 1:
                registarConfiguracao(&sys->configuracoes, &sys->pilhaConfiguracoes);
                break;
            case 2:
                reverterUltimaConfiguracao(&sys->pilhaConfiguracoes);
                break;
            case 3:
                consultarUltimaConfiguracao(sys->pilhaConfiguracoes);
                break;
            case 4: {
                int n = lerInteiro("Número de configurações a consultar: ");
                consultarNConfiguracoes(sys->configuracoes, n);
                break;
            }
            case 5: {
                int codigo = lerInteiro("Código do equipamento a consultar: ");
                consultarHistoricoEquipamento(sys->configuracoes, codigo);
                break;
            }
            case 6:
                limparConfiguracoes(&sys->configuracoes, &sys->pilhaConfiguracoes);
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
