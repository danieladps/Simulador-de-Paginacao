#ifndef TLB_H
#define TLB_H

#include "simulador.h"

void inicializar_tlb(Simulador *sim);
int buscar_tlb(Simulador *sim, int pid, int pagina);
void inserir_tlb(Simulador *sim, int pid, int pagina, int frame);
void invalidar_tlb(Simulador *sim, int pid, int pagina);

#endif
