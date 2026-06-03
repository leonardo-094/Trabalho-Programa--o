#include "relatorios.h"

// Cria um ficheiro de texto com o estado atual da rede, equipamentos e incidentes
void gerarRelatorioEstado(const Equipamento *equipamentos, const Incidente *incidentes, const LeituraSensor *leituras) {
    char nomeFicheiro[128];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(nomeFicheiro, sizeof(nomeFicheiro), "dados/relatorio_estado_rede_%m_%Y.txt", tm);
    FILE *f = fopen(nomeFicheiro, "w");
    if (!f) {
        printf("Não foi possível criar o ficheiro de relatório de estado.\n");
        return;
    }

    int totalEquipamentos = 0;
    int operacionais = 0;
    int emFalha = 0;
    int emManutencao = 0;
    int desativado = 0;
    const Equipamento *eq = equipamentos;
    while (eq) {
        totalEquipamentos++;
        if (strcmp(eq->estado, "Operacional") == 0) {
            operacionais++;
        } else if (strcmp(eq->estado, "Em Falha") == 0) {
            emFalha++;
        } else if (strcmp(eq->estado, "Em Manutenção") == 0) {
            emManutencao++;
        } else if (strcmp(eq->estado, "Desativado") == 0) {
            desativado++;
        }
        eq = eq->next;
    }

    int totalIncidentes = 0;
    int pendentes = 0;
    const Incidente *inc = incidentes;
    while (inc) {
        totalIncidentes++;
        if (strcmp(inc->estado, "Pendente") == 0) {
            pendentes++;
        }
        inc = inc->next;
    }

    int leiturasAnomalias = 0;
    const LeituraSensor *ls = leituras;
    while (ls) {
        if (strcmp(ls->estado, "AVISO") == 0 || strcmp(ls->estado, "CRITICO") == 0 || strcmp(ls->estado, "FALHA_REDE") == 0) {
            leiturasAnomalias++;
        }
        ls = ls->next;
    }

    fprintf(f, "Relatório de estado da rede\n");
    fprintf(f, "===========================\n");
    fprintf(f, "Total de equipamentos: %d\n", totalEquipamentos);
    fprintf(f, "Equipamentos operacionais: %d\n", operacionais);
    fprintf(f, "Equipamentos em falha: %d\n", emFalha);
    fprintf(f, "Equipamentos em manutenção: %d\n", emManutencao);
    fprintf(f, "Equipamentos desativados: %d\n", desativado);
    fprintf(f, "Total de incidentes: %d\n", totalIncidentes);
    fprintf(f, "Incidentes pendentes: %d\n", pendentes);
    fprintf(f, "Leituras anómalas de sensores: %d\n", leiturasAnomalias);

    fprintf(f, "\nLista de equipamentos:\n");
    fprintf(f, "Código | Nome | IP | Estado\n");
    fprintf(f, "-------------------------------------------\n");
    eq = equipamentos;
    while (eq) {
        fprintf(f, "%d | %s | %s | %s\n", eq->codigo, eq->nome, eq->ip, eq->estado);
        eq = eq->next;
    }

    fclose(f);
    printf("Relatório de estado gerado em %s\n", nomeFicheiro);
}

// Cria um ficheiro de texto com o resumo mensal dos incidentes
void gerarRelatorioMensal(const Incidente *incidentes) {
    char nomeFicheiro[128];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(nomeFicheiro, sizeof(nomeFicheiro), "dados/relatorio_incidentes_mensal_%m_%Y.txt", tm);
    FILE *f = fopen(nomeFicheiro, "w");
    if (!f) {
        printf("Não foi possível criar o ficheiro de relatório mensal.\n");
        return;
    }

    int totalIncidentes = 0;
    int prioridadeAlta = 0;
    int prioridadeMedia = 0;
    int prioridadeBaixa = 0;
    const Incidente *inc = incidentes;
    while (inc) {
        totalIncidentes++;
        if (strcmp(inc->prioridade, "Alta") == 0 || strcmp(inc->prioridade, "CRITICO") == 0) {
            prioridadeAlta++;
        } else if (strcmp(inc->prioridade, "Media") == 0 || strcmp(inc->prioridade, "Média") == 0 || strcmp(inc->prioridade, "Aviso") == 0) {
            prioridadeMedia++;
        } else {
            prioridadeBaixa++;
        }
        inc = inc->next;
    }

    fprintf(f, "Relatório mensal de incidentes\n");
    fprintf(f, "===============================\n");
    fprintf(f, "Total de incidentes: %d\n", totalIncidentes);
    fprintf(f, "Prioridade alta/CRITICO: %d\n", prioridadeAlta);
    fprintf(f, "Prioridade média/AVISO: %d\n", prioridadeMedia);
    fprintf(f, "Prioridade baixa: %d\n", prioridadeBaixa);

    fclose(f);
    printf("Relatório mensal gerado em %s\n", nomeFicheiro);
}
