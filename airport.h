#ifndef AIRPORT_H
#define AIRPORT_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <string.h>

#define MAX 50
#define MAX_READ 200

typedef struct passageiro
{
    unsigned int id;
    char nome_passageiro[MAX];
    char cpf[MAX];
    int classe;
    unsigned int assento;
    bool overbooking;
} p;

typedef struct voo
{
    unsigned int id;
    char nome_voo[MAX];
    unsigned int capacidade;
    char destino[MAX_READ];
    char horario[6];
    float preco; 
    float preco_classe;
} v;

void cadastrar_voo();
void historico_voo();
void cadastrar_passageiro();
void confirmar_voo();
void editar_voo();
void editar_passageiro();

#endif
