#include <stdio.h>
#include "simulador.h"
#include "memoria.h"
#include "processo.h"
#include "tlb.h"

int main() {
    Simulador *sim = inicializar_simulador(4096, 16384);
    sim->algoritmo = 1; // 0=FIFO, 1=LRU

    inicializar_tlb(sim);

    Processo *p1 = criar_processo(sim, 16384); // 4 páginas
    Processo *p2 = criar_processo(sim, 16384);  
    Processo *p3 = criar_processo(sim, 16384)

    // Sequência de acessos
    traduzir_endereco(sim, 1, 1000);
    traduzir_endereco(sim, 2, 2000);
    traduzir_endereco(sim, 3, 3000);

    traduzir_endereco(sim, 1, 5000);
    traduzir_endereco(sim, 2, 6000);
    traduzir_endereco(sim, 3, 7000);

    traduzir_endereco(sim, 1, 9000);
    traduzir_endereco(sim, 2, 10000);
    traduzir_endereco(sim, 3, 11000);

    traduzir_endereco(sim, 1, 13000);
    traduzir_endereco(sim, 2, 14000);
    traduzir_endereco(sim, 3, 15000);

    exibir_memoria_fisica(sim);
    exibir_estatisticas(sim);
    liberar_simulador(sim);

    return 0;
}
