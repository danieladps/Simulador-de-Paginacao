#include <stdlib.h>
#include <stdio.h>
#include "simulador.h"
#include "tlb.h"
#include "memoria.h"
#include "processo.h"

Simulador* inicializar_simulador(int tam_pagina, int tam_memoria) {
    // Inicializa o simulador, é a primeira função a ser chamaada
    Simulador *sim = malloc(sizeof(Simulador));
    sim->tamanho_pagina = tam_pagina; // tam_pagina é definida na entrada
    sim->tamanho_memoria_fisica = tam_memoria; // Simula o tamanho de uma memória física
    sim->tempo_atual = 0;
    sim->total_acessos = 0;
    sim->page_faults = 0;
    sim->num_processos = 0;
    sim->processos = NULL;

    int num_frames = tam_memoria / tam_pagina; // define o numero de frames na memoria, com base no tamanho das paginas e na memoria disponivel
    sim->memoria.num_frames = num_frames;
    sim->memoria.frames = calloc(num_frames, sizeof(int)); //aloca os frames na memoria
    sim->memoria.tempo_carga = calloc(num_frames, sizeof(int));

    return sim;
}

void liberar_simulador(Simulador *sim) {
    int i;
    //libera a memória dos processos, dos frames e do tempo de carga de cada frame ao final da simulação
    for (i = 0; i < sim->num_processos; i++) {
        free(sim->processos[i].tabela_paginas);
    }

    free(sim->processos);
    free(sim->memoria.frames);
    free(sim->memoria.tempo_carga);
    free(sim);
}

void extrair_pagina_deslocamento(Simulador *sim, int endereco_virtual, int *pagina, int *deslocamento) {
    *pagina = endereco_virtual/ sim->tamanho_pagina;
    *deslocamento = endereco_virtual % sim->tamanho_pagina;
}

int verificar_pagina_presente(Simulador *sim, int pid, int pagina) {
    Processo *proc = &sim->processos[pid - 1];
    if (proc->tabela_paginas[pagina].presente) {
        return proc->tabela_paginas[pagina].frame;
    }
    return -1;
}

int carregar_pagina(Simulador *sim, int pid, int pagina) {
    int i;
    MemoriaFisica *mem = &sim->memoria;
    int frame_livre = -1;

    for (i = 0; i < mem->num_frames; i++) {
        if (mem->frames[i] == 0)  {
            frame_livre = i;
            break;
        }
    }

    if (frame_livre == -1) {
        switch (sim->algoritmo) {
            case 0:
                frame_livre = substituir_pagina_fifo(sim);
                break;
            case 1:
                frame_livre = substituir_pagina_lru(sim);
                break;
            default:
                printf("Erro: algoritmo de substituição inválido\n");
                return -1;
        }
    }

    mem->frames[frame_livre] = (pid << 16) | pagina;
    mem->tempo_carga[frame_livre] = sim->tempo_atual;

    Processo *proc = &sim->processos[pid - 1];
    proc->tabela_paginas[pagina].presente = 1;
    proc->tabela_paginas[pagina].frame = frame_livre;
    proc->tabela_paginas[pagina].tempo_carga = sim->tempo_atual;
    proc->tabela_paginas[pagina].ultimo_acesso = sim->tempo_atual;

    return frame_livre;

}

int traduzir_endereco(Simulador *sim, int pid, int endereco_virtual) {
    int pagina, deslocamento;
    extrair_pagina_deslocamento(sim, endereco_virtual, &pagina, &deslocamento);

    sim->total_acessos++;
    sim->tempo_atual++;

    Processo *proc = &sim->processos[pid - 1];

    if (pagina >= proc->num_paginas) {
        printf("Erro: endereço virtual inválido\n");
        return -1;
    }

    int frame = buscar_tlb(sim, pid, pagina);

    if (frame != -1) {
        int endereco_fisico = frame * sim->tamanho_pagina + deslocamento;
        printf("Tempo t=%d: [TLB HIT] P%d Página: %d -> Frame: %d -> Endereco Fisico: %d\n", sim->tempo_atual, pid, pagina, frame, endereco_fisico);

        proc->tabela_paginas[pagina].ultimo_acesso = sim->tempo_atual;
        proc->tabela_paginas[pagina].referenciada = 1;
        
        
        return endereco_fisico;
    }

    frame = verificar_pagina_presente(sim, pid, pagina);
    if (frame == -1) {
        printf("Tempo t=%d: [PAGE FAULT] P%d, página %d\n", sim->tempo_atual, pid, pagina);
        sim->page_faults++;
        frame = carregar_pagina(sim, pid, pagina);
        inserir_tlb(sim, pid, pagina, frame);
        printf("Tempo t=%d: Página %d de P%d carregada no frame %d\n", sim->tempo_atual, pagina, pid, frame);
    }

    int endereco_fisico = frame * sim->tamanho_pagina + deslocamento;
    printf("Tempo t=%d: P%d Endereco Virtual: %d -> Página: %d -> Frame: %d -> Endereco Fisico: %d\n", sim->tempo_atual, pid, endereco_virtual, pagina, frame, endereco_fisico);
    
    return endereco_fisico;
}

void exibir_memoria_fisica(Simulador *sim) {
    printf("Estado da memoria fisica: \n");
    int i;
    for (i = 0; i < sim->memoria.num_frames; i++) {
        int val = sim->memoria.frames[i];
        if (val == 0) {
            printf("Frame %d: Livre\n", i);
        } else {
            int pid = val >> 16;
            int pagina = val & 0xFFFF;
            printf("Frame %d: P%d-%d\n", i, pid, pagina);
        }
    }
}

void exibir_estatisticas(Simulador *sim) {
    printf("\n======== ESTATISTICAS ========\n");
    printf("Total de acessos: %d\n", sim->total_acessos);
    printf("Page Faults: %d\n", sim->page_faults);
    float taxa = sim->total_acessos == 0 ? 0.0 : (float)sim->page_faults / sim->total_acessos * 100.0;
    printf("Taxa de Page Faults: %.2f%%\n", taxa);

    printf("TLB Hits: %d\n", sim->tlb_hits);
    printf("TLB Misses: %d\n", sim->tlb_misses);
    float taxa_tlb = (sim-> tlb_hits + sim->tlb_misses) == 0 ? 0.0 :
        (float)sim->tlb_hits / (sim->tlb_hits + sim->tlb_misses) * 100.0;
    printf("Taxa de TLB Hits: %.2f%%\n", taxa_tlb);
}
