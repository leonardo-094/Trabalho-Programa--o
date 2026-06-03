#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "equipamentos.h"
#include "incidentes.h"
#include "sensores.h"

// cria um relatório com o estado geral da rede
void gerarRelatorioEstado(const Equipamento *equipamentos, const Incidente *incidentes, const LeituraSensor *leituras);
// cria um relatório mensal com a distribuição de incidentes por prioridade
void gerarRelatorioMensal(const Incidente *incidentes);

#endif // RELATORIOS_H
