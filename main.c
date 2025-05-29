#include <stdio.h>
#include "simulador.h"

int main() {
    Simulador *sim = inicializar_simulador(4096, 16384);
    inicializar_tlb(sim);
    criar_processo(sim, 16384);
    criar_processo(sim, 8192);

    sim->algoritmo = 1;

    traduzir_endereco(sim, 1, 1234);
    traduzir_endereco(sim, 1, 4321);
    traduzir_endereco(sim, 1, 8192);


    exibir_mermoria_fisica(sim);
    exibir_estatisticas(sim);
    liberar_simulador(sim);
    return 0;
}
