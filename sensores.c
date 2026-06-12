#include "sensores.h"
#include "incidentes.h"
#include <curl/curl.h>

// Ficheiro onde as leituras de sensores são guardadas
static const char *LEITURAS_FILE = "dados/leituras_sensores.dat";

// Insere uma leitura no fim da lista ligada.
static void appendLeitura(LeituraSensor **lista, LeituraSensor *novo) {
    if (!*lista) {
        *lista = novo;
    } else {
        LeituraSensor *atual = *lista;
        while (atual->next) {
            atual = atual->next;
        }
        atual->next = novo;
    }
}

// Adiciona uma leitura à lista de sensores.
void adicionarLeitura(LeituraSensor **lista, LeituraSensor *novo) {
    if (!novo) return;
    appendLeitura(lista, novo);
}

// Descarrega as leituras da API para o ficheiro de sensores.
void descarregarSensores(void) {
    CURL *curl;
    FILE *fp;
    CURLcode result;

    result = curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        fp = fopen("dados/sensores_rack.txt", "w");
        if (!fp) {
            printf("Não foi possível abrir dados/sensores_rack.txt para escrita.\n");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, "https://sensorlab.innominatum.pt/v1/sensors/export/legacy");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
        } else {
            printf("Leituras descarregadas com sucesso para dados/sensores_rack.txt.\n");
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Não foi possível inicializar a biblioteca cURL.\n");
    }

    curl_global_cleanup();
}

// Carrega as leituras guardadas em ficheiro binário
void sensores_carregar(LeituraSensor **lista) {
    FILE *f = fopen(LEITURAS_FILE, "rb");
    if (!f) return;
    LeituraSensorData data;
    while (fread(&data, sizeof(LeituraSensorData), 1, f) == 1) {
        LeituraSensor *novo = malloc(sizeof(LeituraSensor));
        if (!novo) break;
        strncpy(novo->codigo, data.codigo, MAX_NOME);
        novo->codigo[MAX_NOME - 1] = '\0';
        strncpy(novo->tipo, data.tipo, MAX_TIPO);
        novo->tipo[MAX_TIPO - 1] = '\0';
        novo->valor = data.valor;
        strncpy(novo->unidade, data.unidade, MAX_UNIDADE);
        novo->unidade[MAX_UNIDADE - 1] = '\0';
        strncpy(novo->estado, data.estado, MAX_ESTADO);
        novo->estado[MAX_ESTADO - 1] = '\0';
        novo->next = NULL;
        appendLeitura(lista, novo);
    }
    fclose(f);
}

// Guarda a lista atual de leituras no ficheiro binário
void sensores_salvar(const LeituraSensor *lista) {
    FILE *f = fopen(LEITURAS_FILE, "wb");
    if (!f) return;
    const LeituraSensor *atual = lista;
    while (atual) {
        LeituraSensorData data;
        strncpy(data.codigo, atual->codigo, MAX_NOME);
        data.codigo[MAX_NOME - 1] = '\0';
        strncpy(data.tipo, atual->tipo, MAX_TIPO);
        data.tipo[MAX_TIPO - 1] = '\0';
        data.valor = atual->valor;
        strncpy(data.unidade, atual->unidade, MAX_UNIDADE);
        data.unidade[MAX_UNIDADE - 1] = '\0';
        strncpy(data.estado, atual->estado, MAX_ESTADO);
        data.estado[MAX_ESTADO - 1] = '\0';
        fwrite(&data, sizeof(LeituraSensorData), 1, f);
        atual = atual->next;
    }
    fclose(f);
}

// Liberta toda a memória ocupada pelas leituras
void liberarLeituras(LeituraSensor **lista) {
    LeituraSensor *atual = *lista;
    while (atual) {
        LeituraSensor *rem = atual;
        atual = atual->next;
        free(rem);
    }
    *lista = NULL;
}

// Lê o ficheiro de sensores, importa as leituras e cria incidentes para valores
void importarSensores(LeituraSensor **leituras, Incidente **incidentes, FilaItem **fila, int *proximoId) {
    FILE *f = fopen("dados/sensores_rack.txt", "r");
    if (!f) {
        printf("Não foi possível abrir o ficheiro dados/sensores_rack.txt\n");
        return;
    }

    char linha[512];
    int contador = 0;
    char dataHora[MAX_DATA];
    agoraFormato(dataHora, MAX_DATA);
    FILE *log = fopen("dados/log_sensores.txt", "a");

    liberarLeituras(leituras);

    while (fgets(linha, sizeof(linha), f)) {
        if (linha[0] == '\n' || linha[0] == '\0') continue;

        char codigo[MAX_NOME];
        char tipo[MAX_TIPO];
        float valor;
        char unidade[MAX_UNIDADE];
        char estado[MAX_ESTADO];

        if (sscanf(linha, "%63[^;];%31[^;];%f;%15[^;];%31[^;\n]", codigo, tipo, &valor, unidade, estado) == 5) {
            LeituraSensor *nova = malloc(sizeof(LeituraSensor));
            if (!nova) break;
            strncpy(nova->codigo, codigo, MAX_NOME);
            nova->codigo[MAX_NOME - 1] = '\0';
            strncpy(nova->tipo, tipo, MAX_TIPO);
            nova->tipo[MAX_TIPO - 1] = '\0';
            nova->valor = valor;
            strncpy(nova->unidade, unidade, MAX_UNIDADE);
            nova->unidade[MAX_UNIDADE - 1] = '\0';
            strncpy(nova->estado, estado, MAX_ESTADO);
            nova->estado[MAX_ESTADO - 1] = '\0';
            nova->next = NULL;
            appendLeitura(leituras, nova);
            contador++;

            if (strcmp(estado, "AVISO") == 0 || strcmp(estado, "CRITICO") == 0 || strcmp(estado, "FALHA_REDE") == 0) {
                char descricao[MAX_DESCRICAO];
                snprintf(descricao, sizeof(descricao), "Leitura anómala do sensor %s: %f %s (estado: %s)", codigo, valor, unidade, estado);
                Incidente *inc = criarIncidente(*proximoId, "Sensores", 0, codigo, tipo, descricao, "Alta");
                if (inc) {
                    (*proximoId)++;
                    adicionarIncidente(incidentes, inc);
                    enfileirarIncidente(fila, inc);
                }
            }
        }
    }

    if (log) {
        fprintf(log, "[%s] Importação de sensores concluída: %d leituras importadas\n", dataHora, contador);
        fclose(log);
    }

    fclose(f);
    printf("Importação concluída: %d leituras importadas.\n", contador);
    printf("Log gravado em dados/log_sensores.txt\n");
}

// Mostra as leituras de sensores registadas
void listarLeituras(const LeituraSensor *lista) {
    if (!lista) {
        printf("Nenhuma leitura registada.\n");
        return;
    }
    printf("\nLeituras de sensores:\n");
    printf("------------------------------------------------------------\n");
    const LeituraSensor *atual = lista;
    while (atual) {
        printf("Código: %s | Tipo: %s | Valor: %.2f %s | Estado: %s\n",
               atual->codigo, atual->tipo, atual->valor, atual->unidade, atual->estado);
        atual = atual->next;
    }
    printf("------------------------------------------------------------\n");
}

// Procura uma leitura pelo código
LeituraSensor *pesquisarSensorPorCodigo(const LeituraSensor *lista, const char *codigo) {
    const LeituraSensor *atual = lista;
    while (atual) {
        if (strcmp(atual->codigo, codigo) == 0) {
            return (LeituraSensor *)atual;
        }
        atual = atual->next;
    }
    return NULL;
}

// Mostra apenas as leituras que foram classificadas
void listarAnomalias(const LeituraSensor *lista) {
    if (!lista) {
        printf("Nenhuma leitura registada.\n");
        return;
    }
    printf("\nLeituras anómalas:\n");
    printf("------------------------------------------------------------\n");
    const LeituraSensor *atual = lista;
    int contador = 0;
    while (atual) {
        if (strcmp(atual->estado, "AVISO") == 0 || strcmp(atual->estado, "CRITICO") == 0 || strcmp(atual->estado, "FALHA_REDE") == 0) {
            printf("Código: %s | Tipo: %s | Valor: %.2f %s | Estado: %s\n",
                   atual->codigo, atual->tipo, atual->valor, atual->unidade, atual->estado);
            contador++;
        }
        atual = atual->next;
    }
    if (contador == 0) {
        printf("Nenhuma leitura anómala encontrada.\n");
    }
    printf("------------------------------------------------------------\n");
}
