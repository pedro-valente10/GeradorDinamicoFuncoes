/* Edmilson Caiaffa Junior 1911784 3WA */
/* Pedro Henrique Padilha Valente 2510382 3WA */

#include <stdio.h>
#include "cria_func.h"

#define cod(b) (codigo[index++] = (unsigned char)(b))

static int copia_dados(unsigned char codigo[], int index, void *origem, int tamanho) {
    unsigned char *ptr = (unsigned char *) origem;
    for (int i = 0; i < tamanho; i++) {
        cod(ptr[i]);
    }
    return index;
}

void cria_func (void* f, DescParam params[], int n, unsigned char codigo[]) {
    int index = 0;
    int registrador_index = 0;
    // prólogo
    cod(0x55);                                              // pushq %rbp
    cod(0x48); cod(0x89); cod(0xe5);                        // movq %rsp, %rbp
    cod(0x48); cod(0x83); cod(0xec); cod(0x20);             // subq $32, %rsp
    cod(0x48); cod(0x89); cod(0x7d); cod(0xf8);             // movq %rdi, -8(%rbp)
    cod(0x48); cod(0x89); cod(0x75); cod(0xf0);             // movq %rsi, -16(%rbp)
    cod(0x48); cod(0x89); cod(0x55); cod(0xe8);             // movq %rdx, -24(%rbp)

    // parametros
    for (int i = 0; i < n; i++) {
        if (params[i].orig_val == FIX) {
            if (params[i].tipo_val == INT_PAR) {
                if (i == 0){ cod(0xbf); }                   // movl $valor, %edi
    
                else if (i == 1){ cod(0xbe); }              // movl $valor, %esi
                
                else if (i == 2){ cod(0xba); }              // movl $valor, %edx
                
                // copia os bytes do inteiro
                index = copia_dados(codigo, index, &params[i].valor.v_int, 4);               
            }
            else if (params[i].tipo_val == PTR_PAR) {
                if (i == 0){ cod(0x48); cod(0xbf); }        // movq $ponteiro, %rdi
                else if (i == 1){ cod(0x48); cod(0xbe); }   // movq $ponteiro, %rsi
                else if (i == 2){ cod(0x48); cod(0xba); }   // movq $ponteiro, %rdx

                // copia os bytes do ponteiro
                index = copia_dados(codigo, index, &params[i].valor.v_ptr, 8);
            }
        }
        else if (params[i].orig_val == PARAM) {
            unsigned char lugar_pilha;

            if (registrador_index == 0) {
                lugar_pilha = 0xf8;                         // -8(%rbp)
            }
            else if (registrador_index == 1) {
                lugar_pilha = 0xf0;                         // -16(%rbp)
            }
            else if (registrador_index == 2) {
                lugar_pilha = 0xe8;                         // -24(%rbp)
            }

            if (params[i].tipo_val == PTR_PAR) {
                cod(0x48);                                  // se for ponteiro vira %rdi/%rsi/%rdx
            }

            if (i == 0) {
                cod(0x8b); cod(0x7d); cod(lugar_pilha);     // mov -lugar_pilha(%rbp), %edi/%rdi
            }
            else if (i == 1) {
                cod(0x8b); cod(0x75); cod(lugar_pilha);     // mov -lugar_pilha(%rbp), %esi/%rsi
            }
            else if (i == 2) {
                cod(0x8b); cod(0x55); cod(lugar_pilha);     // mov -lugar_pilha(%rbp), %edx/%rdx
            }

            registrador_index++;
        }
        else if (params[i].orig_val == IND) {
            cod(0x48); cod(0xb8);                           // movq $ponteiro, %rax

            // copia os bytes do ponteiro
            index = copia_dados(codigo, index, &params[i].valor.v_ptr, 8);
            
            if (params[i].tipo_val == PTR_PAR) {
                cod(0x48);                                  // se for ponteiro vira %rdi/%rsi/%rdx
            }
            if (i == 0) {
                cod(0x8b); cod(0x38);                       // mov (%rax), %edi/%rdi
            }
            else if (i == 1) {
                cod(0x8b); cod(0x30);                       // mov (%rax), %esi/%rsi
            }
            else if (i == 2) {
                cod(0x8b); cod(0x10);                       // mov (%rax), %edx/%rdx
            }
        }
    }

    // final
    cod(0x48); cod(0xb8);                                   // movq $ponteiro, %rax
    index = copia_dados(codigo, index, &f, 8);
    cod(0xff); cod(0xd0);                                   // call *%rax
    cod(0xc9);                                              // leave
    cod(0xc3);                                              // ret
}