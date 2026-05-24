#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "equipamentos.h"
#include "incidentes.h"
#include "sensores.h"

void gerarRelatorioEstado(const Equipamento *equipamentos, const Incidente *incidentes, const LeituraSensor *leituras);
void gerarRelatorioMensal(const Incidente *incidentes);

#endif // RELATORIOS_H
