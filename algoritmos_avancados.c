#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.
// Estrutura da sala (nó da árvore)

typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// Cria dinamicamente uma sala
Sala* criarSala(char *nome) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    s->esq = NULL;
    s->dir = NULL;
    return s;
}

// O jogador navega pela árvore
void explorarSalas(Sala *atual) {
    char opcao;

    while (atual != NULL) {
        printf("\n📍 Você está em: %s\n", atual->nome);

        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Fim do caminho! Encontrou um beco sem saída.\n");
            return;
        }

        printf("Escolha o caminho: (e) esquerda | (d) direita | (s) sair\n");
        scanf(" %c", &opcao);

        if (opcao == 'e') {
            atual = atual->esq;
        } else if (opcao == 'd') {
            atual = atual->dir;
        } else if (opcao == 's') {
            return;
        } else {
            printf("Opção inválida.\n");
        }
    }
}


// =================================================================
// 🧠 NÍVEL AVENTUREIRO – ÁRVORE DE BUSCA (ORGANIZAÇÃO DAS PISTAS)
// =================================================================

// BST de pistas
typedef struct NodoPista {
    char pista[50];
    struct NodoPista *esq;
    struct NodoPista *dir;
} NodoPista;

// Cria nó da BST
NodoPista* novoNodoPista(char *p) {
    NodoPista *n = (NodoPista*) malloc(sizeof(NodoPista));
    strcpy(n->pista, p);
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

// Inserção na BST
NodoPista* inserirPista(NodoPista *raiz, char *p) {
    if (raiz == NULL)
        return novoNodoPista(p);

    if (strcmp(p, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, p);
    else
        raiz->dir = inserirPista(raiz->dir, p);

    return raiz;
}

// Listagem em ordem alfabética
void listarPistasEmOrdem(NodoPista *raiz) {
    if (raiz == NULL) return;
    listarPistasEmOrdem(raiz->esq);
    printf("🔎 %s\n", raiz->pista);
    listarPistasEmOrdem(raiz->dir);
}


// =================================================================
// 🏆 NÍVEL MESTRE – TABELA HASH (PISTA → SUSPEITO)
// =================================================================

#define TAM_HASH 26   // índice A–Z

typedef struct Assoc {
    char pista[50];
    char suspeito[50];
    struct Assoc *prox;
} Assoc;

Assoc *tabela[TAM_HASH];

// Função hash simples: usa a primeira letra
int hash(char *str) {
    char c = str[0];
    if (c >= 'a' && c <= 'z') c -= 32;
    return (c - 'A') % TAM_HASH;
}

void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        tabela[i] = NULL;
    }
}

void inserirNaHash(char *pista, char *suspeito) {
    int h = hash(pista);

    Assoc *nova = (Assoc*) malloc(sizeof(Assoc));
    strcpy(nova->pista, pista);
    strcpy(nova->suspeito, suspeito);
    nova->prox = tabela[h];
    tabela[h] = nova;
}

void listarHash() {
    printf("\n📚 Relação de Pistas → Suspeitos:\n");

    for (int i = 0; i < TAM_HASH; i++) {
        Assoc *a = tabela[i];
        while (a != NULL) {
            printf("• \"%s\" → %s\n", a->pista, a->suspeito);
            a = a->prox;
        }
    }
}

char* suspeitoMaisCitado() {
    // Contagem por suspeito
    typedef struct {
        char nome[50];
        int freq;
    } Cont;

    Cont lista[20];
    int total = 0;

    // Percorre a hash
    for (int i = 0; i < TAM_HASH; i++) {
        for (Assoc *a = tabela[i]; a != NULL; a = a->prox) {

            int achou = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(lista[j].nome, a->suspeito) == 0) {
                    lista[j].freq++;
                    achou = 1;
                    break;
                }
            }

            if (!achou) {
                strcpy(lista[total].nome, a->suspeito);
                lista[total].freq = 1;
                total++;
            }
        }
    }

    // Encontrar o mais citado
    int max = -1;
    char *res = NULL;

    for (int i = 0; i < total; i++) {
        if (lista[i].freq > max) {
            max = lista[i].freq;
            res = lista[i].nome;
        }
    }

    return res;
}


// =================================================================
// 🔚 MAIN — Junta tudo num mini–jogo
// =================================================================

int main() {

    // -----------------------------------------------------------------
    // NÍVEL NOVATO — Criar mapa fixo da mansão (árvore binária)
    // -----------------------------------------------------------------

    Sala *hall = criarSala("Hall de Entrada");
    hall->esq = criarSala("Biblioteca");
    hall->dir = criarSala("Sala de Jantar");

    hall->esq->esq = criarSala("Arquivo Antigo");
    hall->esq->dir = criarSala("Sala Secreta");

    hall->dir->esq = criarSala("Cozinha");
    hall->dir->dir = criarSala("Porão Escuro");

    // -----------------------------------------------------------------
    // NÍVEL AVENTUREIRO — Árvore de busca (pistas)
    // -----------------------------------------------------------------
    NodoPista *pistas = NULL;

    // -----------------------------------------------------------------
    // NÍVEL MESTRE — Tabela hash
    // -----------------------------------------------------------------
    inicializarHash();

    char opcao;

    do {
        printf("\n=====================================");
        printf("\n🔍 DETECTIVE QUEST — MENU PRINCIPAL");
        printf("\n=====================================");
        printf("\n1 - Explorar a mansão");
        printf("\n2 - Ver pistas encontradas (BST)");
        printf("\n3 - Mostrar relações pista → suspeito (Hash)");
        printf("\n4 - Mostrar suspeito mais citado");
        printf("\n0 - Sair");
        printf("\nEscolha: ");
        scanf(" %c", &opcao);

        switch(opcao) {

        case '1': {
            printf("\n⚠ Ao explorar, você encontrará pistas automaticamente!\n");

            Sala *ptr = hall;
            char escolha;

            while (ptr != NULL) {
                printf("\nVocê está em: %s\n", ptr->nome);

                // adiciona pistas em algumas salas
                if (strcmp(ptr->nome, "Biblioteca") == 0) {
                    pistas = inserirPista(pistas, "Página Rasgada");
                    inserirNaHash("Página Rasgada", "Sr. Black");
                } 
                if (strcmp(ptr->nome, "Sala Secreta") == 0) {
                    pistas = inserirPista(pistas, "Chave Dourada");
                    inserirNaHash("Chave Dourada", "Sra. White");
                }
                if (strcmp(ptr->nome, "Cozinha") == 0) {
                    pistas = inserirPista(pistas, "Faca Faltando");
                    inserirNaHash("Faca Faltando", "Chef Bruno");
                }
                if (strcmp(ptr->nome, "Porão Escuro") == 0) {
                    pistas = inserirPista(pistas, "Pegada Suja");
                    inserirNaHash("Pegada Suja", "Sr. Black");
                }

                if (ptr->esq == NULL && ptr->dir == NULL) break;

                printf("Caminho: (e) esquerda | (d) direita | (s) sair\n");
                scanf(" %c", &escolha);

                if (escolha == 'e') ptr = ptr->esq;
                else if (escolha == 'd') ptr = ptr->dir;
                else break;
            }

            break;
        }

        case '2':
            printf("\n📜 Pistas encontradas (em ordem):\n");
            listarPistasEmOrdem(pistas);
            break;

        case '3':
            listarHash();
            break;

        case '4': {
            char *sus = suspeitoMaisCitado();
            if (sus)
                printf("\n👤 Suspeito mais citado: %s\n", sus);
            else
                printf("\nNenhum suspeito registrado.\n");
            break;
        }

        case '0':
            printf("\nEncerrando investigação...\n");
            break;

        default:
            printf("\nOpção inválida!\n");
        }

    } while (opcao != '0');

    return 0;
}
