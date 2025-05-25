#include <stdlib.h>
#include <stdio.h>
#include "simulador.h"

Simulador* inicializar_simulador(int tam_pagina, int tam_mermoria) {
    Simulador *sim = malloc(sizeof(Simulador));
    sim->tamanho_pagina = tam_pagina;
    sim->tamanho_memoria_fisica = tam_memoria;
    sim->tempo_atual = 0;
    sim->total_acessos = 0;
    sim->page_faults = 0;
    sim->num_processos = 0;
    sim->processos = NULL;

    int num_frames = tam_memoria / tam_pagina;
    sim->memoria.num_frames = num_frames;
    sim->memoria.frames = calloc(num_frames, sizeof(int));
    sim->memoria.tempo_carga = calloc(num_frames, sizeof(int));

    return sim;
}

void liberar_simulador(Simulador *sim) {
    for (int i = 0; i < sim->num_processos; i++) {
        free(sim->processos[i].tabela_paginas);
    }

    free(sim->processos);
    free(sim->memoria.frames);
    free(sim->memoria.tempo_carga);
    free(sim);
}

void extrair_pagina_deslocamento(SImulador *sim, int endereco_virtual, int *pagina, int *deslocamento) {
    *pagina = endereco_virtual/ sim->tamanho_paginas
    *deslocamento = endereco_virtual % sim->tamanho_paginas
}

int verificar_pagina_presente(Simulador *sim, int pid, int pagina) {
    Processo *proc = &sim->processos[pid - 1];
    if (proc->tabela_paginas[pagina].presente) {
        return proc->tabela_paginas[pagina].frame;
    }
    return -1;
}

int carregar_pagina(Simulador *sim, int pid, int pagina) {
    MemoriaFisica *mem = &sim->memoria;
    int frame_livre = -1;

    for (int i = 0; i < mem->num_frames; i++) {
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
    proc->tabela_paginas[pagina].tampo_carga = sim->tempo_atual;
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

    int frame = verificar_pagina_presente(sim, pid, pagina);

    if (frame == -1) {
        printf("Tempo t=%d: [PAGE FAULT] P%d, página %d\n", sim->tempo_atual, pid, pagina);
        sim->page_faults++;
        frame = carregar_pagina(sim, pid, pagina);
        printf("Tempo t=%d: Página %d de P%d carregada no frame %d\n", sim->tempo_atual, pagina, pid, frame);
    }

    int endereco_fisico = frame * sim->tamanho_pagina + deslocamento;
    printf("Tempo t=%d: P%d Endereco Virtual: %d -> Página: %d -> Frame: %d -> Endereco Fisico: %d\n", sim->tempo_atual, pid, endereco_virtual, pagina, frame, endereco_fisico);

    return endereco_fisico;
}
void exibir_memoria_fisica(Simulador *sim) {
    printf("Estado da memoria fisica: \n")
    for (int i = 0; i < sim->memoria.num_frames; i++) {
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
    printf("\n======== ESTATÍSTICAS ========\n");
    printf("Total de acessos: %d\n", sim->total_acessos);
    printf("Page Faults: %d\n", sim->page_faults);
    float taxa = sim->total_acessos == 0 ? 0.0 : (float)sim->page_faults / sim->total_acessos * 100.0;
    printf("Taxa de Page Faults: %.2f%%\n", taxa);
}
