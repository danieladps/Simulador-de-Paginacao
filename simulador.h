#ifndef SIMULADOR_H
#define SIMULADOR_H
#define TLB_TAMANHO 4


typedef struct {
    int presente;
    int frame;
    int modificada;
    int referenciada;
    int tempo_carga;
    int ultimo_acesso;
} Pagina;

typedef struct {
    int pid;
    int tamanho;
    int num_paginas;
    Pagina *tabela_paginas;
} Processo;

typedef struct {
    int num_frames;
    int *frames;
    int *tempo_carga;
} MemoriaFisica;

typedef struct {
    int pid;
    int pagina;
    int frame;
    int tempo_carga;
    int ultimo_acesso;
    int valido;
} EntradaTLB;

typedef struct {
    int tempo_atual;
    int tamanho_pagina;
    int tamanho_memoria_fisica;
    int num_processos;
    Processo *processos;
    MemoriaFisica memoria;

    int total_acessos;
    int page_faults;

    EntradaTLB tlb[TLB_TAMANHO];
    int tlb_hits;
    int tlb_misses;

    int algoritmo;
} Simulador;

Simulador* inicializar_simulador(int tam_pagina, int tam_memoria);
Processo* criar_processo(Simulador *sim, int tamanho_processo);
void liberar_simulador(Simulador *sim);
void exibir_memoria_fisica(Simulador *sim);
void exibir_estatisticas(Simulador *sim);

#endif 

