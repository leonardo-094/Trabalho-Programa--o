#ifndef UTILS_H
#define UTILS_H

// Bibliotecas usadas no projeto.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Tamanhos máximos das strings
#define MAX_NOME 64
#define MAX_TIPO 32
#define MAX_MARCA 32
#define MAX_MODELO 32
#define MAX_IP 32
#define MAX_MAC 32
#define MAX_LOCALIZACAO 64
#define MAX_ESTADO 32
#define MAX_DATA 32
#define MAX_DESCRICAO 128
#define MAX_PRIORIDADE 16
#define MAX_TECNICO 32
#define MAX_UNIDADE 16

// Funções auxiliares 
void limparBuffer(void);
int lerInteiro(const char *prompt);
void lerTexto(const char *prompt, char *destino, int tamanho);
char *agoraFormato(char *buffer, int tamanho);

#endif // UTILS_H
