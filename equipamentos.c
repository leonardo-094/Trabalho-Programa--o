#include "equipamentos.h"
#include "incidentes.h"

// Ficheiro onde os equipamentos são guardados
static const char *EQUIPAMENTOS_FILE = "dados/equipamentos.dat";

// Verifica se o estado é válido
static bool validarEstadoEquipamento(const char *estado) {
    return strcmp(estado, "Operacional") == 0
        || strcmp(estado, "Em Falha") == 0
        || strcmp(estado, "Em Manutenção") == 0
        || strcmp(estado, "Em Manutencao") == 0
        || strcmp(estado, "Desativado") == 0;
}

// Ajusta o texto do estado para o formato correto 
static void normalizarEstadoEquipamento(char *dest, const char *estado) {
    if (strcmp(estado, "Em Manutencao") == 0) {
        strncpy(dest, "Em Manutenção", MAX_ESTADO);
    } else {
        strncpy(dest, estado, MAX_ESTADO);
    }
    dest[MAX_ESTADO - 1] = '\0';
}

// Inserir um equipamento no final da lista
static void appendEquipamento(Equipamento **lista, Equipamento *novo) {
    if (!*lista) {
        *lista = novo;
    } else {
        Equipamento *atual = *lista;
        while (atual->next) {
            atual = atual->next;
        }
        atual->next = novo;
    }
}

// Cria um novo equipamento fornecidos pelo utilizador.
Equipamento *criarEquipamento(int codigo) {
    Equipamento *novo = malloc(sizeof(Equipamento));
    if (!novo) return NULL;
    novo->codigo = codigo;
    lerTexto("Nome do equipamento: ", novo->nome, MAX_NOME);
    lerTexto("Tipo do equipamento: ", novo->tipo, MAX_TIPO);
    lerTexto("Marca: ", novo->marca, MAX_MARCA);
    lerTexto("Modelo: ", novo->modelo, MAX_MODELO);
    lerTexto("Endereço IP: ", novo->ip, MAX_IP);
    lerTexto("Endereço MAC: ", novo->mac, MAX_MAC);
    lerTexto("Localização física: ", novo->localizacao, MAX_LOCALIZACAO);
    strcpy(novo->estado, "Operacional");
    agoraFormato(novo->ultimaVerificacao, MAX_DATA);
    novo->next = NULL;
    return novo;
}

// Adiciona um equipamento à lista e mostra uma mensagem de confirmação.
void adicionarEquipamento(Equipamento **lista, Equipamento *novo) {
    if (!novo) return;
    appendEquipamento(lista, novo);
    printf("Equipamento %s (%d) adicionado com sucesso.\n", novo->nome, novo->codigo);
}

// Procura um equipamento pelo seu código numérico
Equipamento *pesquisarPorCodigo(const Equipamento *lista, int codigo) {
    const Equipamento *atual = lista;
    while (atual) {
        if (atual->codigo == codigo) {
            return (Equipamento *)atual;
        }
        atual = atual->next;
    }
    return NULL;
}

// Procura um equipamento pelo endereço IP
Equipamento *pesquisarPorIP(const Equipamento *lista, const char *ip) {
    const Equipamento *atual = lista;
    while (atual) {
        if (strcmp(atual->ip, ip) == 0) {
            return (Equipamento *)atual;
        }
        atual = atual->next;
    }
    return NULL;
}

// Procura um equipamento pelo endereço MAC
Equipamento *pesquisarPorMAC(const Equipamento *lista, const char *mac) {
    const Equipamento *atual = lista;
    while (atual) {
        if (strcmp(atual->mac, mac) == 0) {
            return (Equipamento *)atual;
        }
        atual = atual->next;
    }
    return NULL;
}

// Remove um equipamento
bool removerEquipamento(Equipamento **lista, const Incidente *incidentes, int codigo) {
    const Incidente *inc = incidentes;
    while (inc) {
        if (inc->codigoEquipamento == codigo && strcmp(inc->estado, "Pendente") == 0) {
            printf("Não é possível remover o equipamento %d: incidente pendente associado.\n", codigo);
            return false;
        }
        inc = inc->next;
    }

    Equipamento *anterior = NULL;
    Equipamento *atual = *lista;
    while (atual && atual->codigo != codigo) {
        anterior = atual;
        atual = atual->next;
    }

    if (!atual) {
        printf("Equipamento %d não encontrado.\n", codigo);
        return false;
    }

    if (anterior) {
        anterior->next = atual->next;
    } else {
        *lista = atual->next;
    }
    free(atual);
    printf("Equipamento %d removido com sucesso.\n", codigo);
    return true;
}

// Altera os dados de um equipamento
bool alterarEquipamento(Equipamento *lista, int codigo) {
    Equipamento *eq = pesquisarPorCodigo(lista, codigo);
    if (!eq) {
        printf("Equipamento %d não encontrado.\n", codigo);
        return false;
    }

    printf("Alterando equipamento %d (%s)\n", eq->codigo, eq->nome);
    lerTexto("Nome do equipamento: ", eq->nome, MAX_NOME);
    lerTexto("Tipo do equipamento: ", eq->tipo, MAX_TIPO);
    lerTexto("Marca: ", eq->marca, MAX_MARCA);
    lerTexto("Modelo: ", eq->modelo, MAX_MODELO);
    lerTexto("Endereço IP: ", eq->ip, MAX_IP);
    lerTexto("Endereço MAC: ", eq->mac, MAX_MAC);
    lerTexto("Localização física: ", eq->localizacao, MAX_LOCALIZACAO);
    char novoEstado[MAX_ESTADO];
    lerTexto("Estado (Operacional/Em Falha/Em Manutenção/Desativado): ", novoEstado, MAX_ESTADO);
    if (!validarEstadoEquipamento(novoEstado)) {
        printf("Estado inválido. Alteração cancelada.\n");
        return false;
    }
    normalizarEstadoEquipamento(eq->estado, novoEstado);
    agoraFormato(eq->ultimaVerificacao, MAX_DATA);
    printf("Equipamento %d alterado com sucesso.\n", codigo);
    return true;
}

// Atualiza apenas o estado de um equipamento
bool alterarEstadoEquipamento(Equipamento *lista, int codigo, const char *novoEstado) {
    Equipamento *eq = pesquisarPorCodigo(lista, codigo);
    if (!eq) {
        return false;
    }
    if (!validarEstadoEquipamento(novoEstado)) {
        return false;
    }
    normalizarEstadoEquipamento(eq->estado, novoEstado);
    agoraFormato(eq->ultimaVerificacao, MAX_DATA);
    return true;
}

// Mostra todos os equipamentos de um determinado tipo
void listarPorTipo(const Equipamento *lista, const char *tipo) {
    if (!lista) {
        printf("Nenhum equipamento registado.\n");
        return;
    }

    printf("\nEquipamentos do tipo %s:\n", tipo);
    printf("------------------------------------------------------------\n");
    const Equipamento *atual = lista;
    int encontrados = 0;
    while (atual) {
        if (strcmp(atual->tipo, tipo) == 0) {
            printf("Código: %d | Nome: %s | Tipo: %s | IP: %s | Estado: %s\n",
                   atual->codigo, atual->nome, atual->tipo, atual->ip, atual->estado);
            encontrados++;
        }
        atual = atual->next;
    }

    if (!encontrados) {
        printf("Nenhum equipamento do tipo %s encontrado.\n", tipo);
    }
    printf("------------------------------------------------------------\n");
}

// Mostra todos os equipamentos com um determinado estado
void listarPorEstado(const Equipamento *lista, const char *estado) {
    if (!lista) {
        printf("Nenhum equipamento registado.\n");
        return;
    }

    printf("\nEquipamentos com estado %s:\n", estado);
    printf("------------------------------------------------------------\n");
    const Equipamento *atual = lista;
    int encontrados = 0;
    while (atual) {
        if (strcmp(atual->estado, estado) == 0) {
            printf("Código: %d | Nome: %s | Tipo: %s | IP: %s | Estado: %s\n",
                   atual->codigo, atual->nome, atual->tipo, atual->ip, atual->estado);
            encontrados++;
        }
        atual = atual->next;
    }

    if (!encontrados) {
        printf("Nenhum equipamento com estado %s encontrado.\n", estado);
    }
    printf("------------------------------------------------------------\n");
}

// Lista todos os equipamentos registados
void listarEquipamentos(const Equipamento *lista) {
    if (!lista) {
        printf("Nenhum equipamento registado.\n");
        return;
    }
    printf("\nEquipamentos registados:\n");
    printf("------------------------------------------------------------\n");
    const Equipamento *atual = lista;
    while (atual) {
        printf("Código: %d | Nome: %s | Tipo: %s | IP: %s | Estado: %s\n",
               atual->codigo, atual->nome, atual->tipo, atual->ip, atual->estado);
        atual = atual->next;
    }
    printf("------------------------------------------------------------\n");
}

// Lê os equipamentos guardados em ficheiro binário e reconstruí a lista
void equipamentos_carregar(Equipamento **lista, int *proximoCodigo) {
    FILE *f = fopen(EQUIPAMENTOS_FILE, "rb");
    if (!f) return;
    EquipamentoData data;
    int maxCodigo = *proximoCodigo - 1;
    while (fread(&data, sizeof(EquipamentoData), 1, f) == 1) {
        Equipamento *novo = malloc(sizeof(Equipamento));
        if (!novo) break;
        novo->codigo = data.codigo;
        strncpy(novo->nome, data.nome, MAX_NOME);
        novo->nome[MAX_NOME - 1] = '\0';
        strncpy(novo->tipo, data.tipo, MAX_TIPO);
        novo->tipo[MAX_TIPO - 1] = '\0';
        strncpy(novo->marca, data.marca, MAX_MARCA);
        novo->marca[MAX_MARCA - 1] = '\0';
        strncpy(novo->modelo, data.modelo, MAX_MODELO);
        novo->modelo[MAX_MODELO - 1] = '\0';
        strncpy(novo->ip, data.ip, MAX_IP);
        novo->ip[MAX_IP - 1] = '\0';
        strncpy(novo->mac, data.mac, MAX_MAC);
        novo->mac[MAX_MAC - 1] = '\0';
        strncpy(novo->localizacao, data.localizacao, MAX_LOCALIZACAO);
        novo->localizacao[MAX_LOCALIZACAO - 1] = '\0';
        strncpy(novo->estado, data.estado, MAX_ESTADO);
        novo->estado[MAX_ESTADO - 1] = '\0';
        strncpy(novo->ultimaVerificacao, data.ultimaVerificacao, MAX_DATA);
        novo->ultimaVerificacao[MAX_DATA - 1] = '\0';
        novo->next = NULL;
        appendEquipamento(lista, novo);
        if (novo->codigo > maxCodigo) {
            maxCodigo = novo->codigo;
        }
    }
    *proximoCodigo = maxCodigo + 1;
    fclose(f);
}

// Guarda a lista atual de equipamentos em ficheiro binário
void equipamentos_salvar(const Equipamento *lista) {
    FILE *f = fopen(EQUIPAMENTOS_FILE, "wb");
    if (!f) return;
    const Equipamento *atual = lista;
    while (atual) {
        EquipamentoData data;
        data.codigo = atual->codigo;
        strncpy(data.nome, atual->nome, MAX_NOME);
        data.nome[MAX_NOME - 1] = '\0';
        strncpy(data.tipo, atual->tipo, MAX_TIPO);
        data.tipo[MAX_TIPO - 1] = '\0';
        strncpy(data.marca, atual->marca, MAX_MARCA);
        data.marca[MAX_MARCA - 1] = '\0';
        strncpy(data.modelo, atual->modelo, MAX_MODELO);
        data.modelo[MAX_MODELO - 1] = '\0';
        strncpy(data.ip, atual->ip, MAX_IP);
        data.ip[MAX_IP - 1] = '\0';
        strncpy(data.mac, atual->mac, MAX_MAC);
        data.mac[MAX_MAC - 1] = '\0';
        strncpy(data.localizacao, atual->localizacao, MAX_LOCALIZACAO);
        data.localizacao[MAX_LOCALIZACAO - 1] = '\0';
        strncpy(data.estado, atual->estado, MAX_ESTADO);
        data.estado[MAX_ESTADO - 1] = '\0';
        strncpy(data.ultimaVerificacao, atual->ultimaVerificacao, MAX_DATA);
        data.ultimaVerificacao[MAX_DATA - 1] = '\0';
        fwrite(&data, sizeof(EquipamentoData), 1, f);
        atual = atual->next;
    }
    fclose(f);
}

// Liberta toda a memória ocupada pela lista de equipamentos
void liberarEquipamentos(Equipamento **lista) {
    Equipamento *atual = *lista;
    while (atual) {
        Equipamento *rem = atual;
        atual = atual->next;
        free(rem);
    }
    *lista = NULL;
}
