#include <stdlib.h>
#include "simulador.h"

Processo* criar_processo(Simulador *sim, int tamanho_processo) {
    sim->num_processos++;
    sim->processos = realloc(sim->processos, sizeof(Processo) * sim->num_processos);

    Processo *p = &sim->processos[sim->num_processos - 1];
    p->pid = sim->num_processos;
    p->tamanho = tamanho_processo;
    p->num_paginas = tamanho_processo / sim->tamanho_pagina;
    p->tabela_paginas = calloc(p->num_paginas, sizeof(Pagina));

    for (int i = 0; i < p->num_paginas; i++) {
        p->tabela_paginas[i].frame = -1;
    }

    return p;
}
