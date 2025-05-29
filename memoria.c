#include "memoria.h"
#include <limits.h>
#include <stdio.h>

int substituir_pagina_fifo(Simulador *sim) {
    int frame_escolhido = -1;
    int tempo_mais_antigo = INT_MAX;


    for (int i = 0; i < sim->memoria.num_frames; i++) {
        if (sim->memoria.tempo_carga[i] < tempo_mais_antigo) {
            tempo_mais_antigo = sim->memoria.tempo_carga[i];
            frame_escolhido = i;
        }
    }

    if (frame_escolhido != -1) {
        int val = sim->memoria.frames[frame_escolhido];
        int old_pid = val >> 16;
        int old_pagina = val & 0xFFFF;

        sim->processos[old_pid - 1].tabela_paginas[old_pagina].presente = 0;
        sim->processos[old_pid - 1].tabela_paginas[old_pagina].frame = -1;

        printf("Tempo t=%d: Substituindo página %d de P%d do frame %d (FIFO)\n", sim->tempo_atual, old_pagina, old_pid, frame_escolhido);
    }

    return frame_escolhido;
}

int substituir_pagina_lru(Simulador *sim) {
    int frame_escolhido = -1;
    int acesso_mais_antigo = INT_MAX;

    for (int i = 0; i < sim->memoria.num_frames; i++) {
        int val = sim->memoria.frames[i];
        if (val == 0) continue;

        int pid = val >> 16;
        int pagina val & 0xFFFF;

        Pagina *pag = &sim->processos[pid - 1].tabela_paginas[pagina];

        if (pag->ultimo_acesso < acesso_mais_antigo) {
            acesso_mais_antigo = pag->ultimo_acesso;
            frame_escolhido = i;
        }
    }

    if (frame_escolhido != -1) {
        int val = sim->memoria.frames[frame_escolhido];
        int old_pid = val >> 16;
        int old_pagina = val & 0xFFFF;

        sim->processos[old_pid - 1].tabela_paginas[old_pagina].presente = 0;
        sim->processos[old_pid - 1].tabela_paginas[old_pagina].frame = -1;

        printf("Tempo t=%d: Substituindo página %d de P%d do frame %d (FIFO)\n", sim->tempo_atual, old_pagina, old_pid, frame_escolhido);
    }
    return frame_escolhido


}
