#include "tlb.h"
#include <stdio.h>

void inicializar_tlb(Simulador *sim) {
    for (int i = 0; i < TLB_TAMANHO; i++) {
        sim->tlb[i].valido = 0;
    }
    sim ->tlb_hits = 0;
    sim->tlb_misses = 0;
}

int buscar_tlb(Simulador *sim, int pid, int pagina) {
    for (int i = 0; i < TLB_TAMANHO; i++) {
        if (sim->tlb[i].valido && sim->tlb[i].pid == pid && sim->tlb[i].pagina == pagina) {
            sim->tlb_hits++;
            return sim->tlb[i].frame;
        }
    }

    sim->tlb_misses++;
    return -1;
}

void inserir_tlb(Simulador *sim, int pid, int pagina, int frame) {
    for (int i = 0; i < TLB_TAMANHO; i++) {
        if (!sim->tlb[i].valido) {
            sim->tlb[i] = (EntradaTLB)(pid, pagina, frame, sim->tempo_atual, 1);
            return;
        }
    }

    int mais_antigo = 0;
    for (int i = 1; i < TLB_TAMANHO; i++) {
        if (sim->tlb[i].tempo_carga < sim->tlb[mais_antigo].tempo_carga) {
            mais_antigo = i;
        }
    }

    sim->tlb[mais_antigo] = (EntradaTLB)(pid, pagina, frame, sim->tempo_atual, 1);
}

void invalidar_tlb(Simulador *sim, int pid, int pagina) {
    for (int i = 0; i < TLB_TAMANHO; i++) {
        if (sim->tlb[i].valido && sim->tlb[i].pid == pid && sim->tlb[i].pagina == pagina) {
            sim->tlb[i].valido = 0;
        }
    }
}
