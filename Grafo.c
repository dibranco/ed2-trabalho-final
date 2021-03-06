#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h" //inclui os Prot�tipos
#include "ListaSequencial.h"

//Defini��o do tipo Grafo
struct grafo{
    int nro_vertices;
    int grau_max;
    int** arestas;
    Lista*** trabalhos;
    int* grau;
};

Grafo* cria_Grafo(int nro_vertices, int grau_max){
    Grafo *gr;
    gr = (Grafo*) malloc(sizeof(struct grafo));
    if(gr != NULL){
        int i;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->grau = (int*) calloc(nro_vertices,sizeof(int));

        gr->arestas = (int**) malloc(nro_vertices * sizeof(int*));
        for(i=0; i<nro_vertices; i++)
            gr->arestas[i] = (int*) malloc(grau_max * sizeof(int));

        Lista*** trabalhos = (Lista***) malloc(nro_vertices * sizeof(Lista**));
        for(i = 0; i < 10; i++){
            trabalhos[i] = (Lista**) malloc(grau_max * sizeof(Lista*));
        }
        gr->trabalhos = trabalhos;

    }
    return gr;
}

void libera_Grafo(Grafo* gr){
    if(gr != NULL){
        int i;
        for(i=0; i<gr->nro_vertices; i++)
            free(gr->arestas[i]);
        free(gr->arestas);
        free(gr->grau);
        free(gr);
    }
}

int insereAresta(Grafo* gr, int orig, int dest, int eh_digrafo, int val){
    if(gr == NULL)
        return 0;
    if(orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if(dest < 0 || dest >= gr->nro_vertices)
        return 0;

    int i;
    int grauOrig = -1;

    for(i = 0; i < gr->grau_max; i++){
        if(gr->arestas[orig][i] == dest){
            grauOrig = i;
        }
    }

    if(grauOrig == -1){
        grauOrig = gr->grau[orig];
        gr->grau[orig]++;
    }

    gr->arestas[orig][grauOrig] = dest;

    if(gr->trabalhos[orig][grauOrig] == NULL){
        gr->trabalhos[orig][grauOrig] = cria_lista();
    };

    insere_lista_final(gr->trabalhos[orig][grauOrig], val);

    if(eh_digrafo == 0)
        insereAresta(gr,dest,orig,1,val);
    return 1;
}

int removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo){
    if(gr == NULL)
        return 0;
    if(orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if(dest < 0 || dest >= gr->nro_vertices)
        return 0;

    int i = 0;
    while(i<gr->grau[orig] && gr->arestas[orig][i] != dest)
        i++;
    if(i == gr->grau[orig])//elemento nao encontrado
        return 0;
    gr->grau[orig]--;
    gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];
    if(eh_digrafo == 0)
        removeAresta(gr,dest,orig,1);
    return 1;
}

void imprime_Grafo(Grafo *gr){
    if(gr == NULL)
        return;

    int i, j;
    for(i=0; i < gr->nro_vertices; i++){
        if(gr->grau[i] > 0){
            printf("%d: ", i);
            for(j=0; j < gr->grau[i]; j++){
                    printf("%d", gr->arestas[i][j]);
                    imprime_lista(gr->trabalhos[i][j]);
                    printf(", ");
            }
            printf("\n");
        }
    }
}

int max_grau(Grafo* gr){
    int i, maxGrau = 0;
    for(i = 0; i < gr->nro_vertices; i++){
        if(gr->grau[i] > maxGrau){
            maxGrau = gr->grau[i];
        }
    }
    return maxGrau;
}
float med_grau(Grafo* gr){
    int i, qtde = 0;
    float total;
    for(i = 0; i < gr->nro_vertices; i++){
        if(gr->grau[i]){
            total += gr->grau[i];
            qtde++;
        }
    }
    return total/qtde;
}
