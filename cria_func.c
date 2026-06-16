/* Pedro Henrique Padilha Valente 2510382 */
/*  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cria_func.h"

void cria_func(void* f, DescParam params[], int n, unsigned char codigo[]) {
    int idx = 0;

    // Prólogo: alinhando a pilha
    codigo[idx++] = 0x55;             // push %rbp
    codigo[idx++] = 0x48; 
    codigo[idx++] = 0x89; 
    codigo[idx++] = 0xe5;             // mov %rsp, %rbp

    // Salvar os parâmetros recebidos (RDI, RSI, RDX) na pilha para não perder o valor original 
    // caso os registradores precisem ser sobrescritos
    codigo[idx++] = 0x57;             // push %rdi
    codigo[idx++] = 0x56;             // push %rsi
    codigo[idx++] = 0x52;             // push %rdx

    int param_recebido_idx = 0;
    
    // Configurar os registradores para a chamada da função f (RDI, RSI, RDX)
    for (int i = 0; i < n; i++) {
        // Exemplo simplificado da lógica de preenchimento de registradores:
        // A implementação real dos opcodes varia de acordo com o registrador alvo (RDI=1º, RSI=2º, RDX=3º)
        // e com o tipo (INT_PAR 32-bits vs PTR_PAR 64-bits).
        
        if (params[i].orig_val == PARAM) {
            // O parâmetro deve ser recebido pela nova função e repassado à função original
            // Recupera-se o valor salvo na pilha correspondente à ordem (param_recebido_idx)
            // e move-se para o registrador correto (RDI, RSI ou RDX).
            param_recebido_idx++;
        } 
        else if (params[i].orig_val == FIX) {
            // O parâmetro deve ser amarrado a um valor constante
            // Deve-se injetar o opcode de 'mov' com o valor imediato extraído de params[i].valor
        } 
        else if (params[i].orig_val == IND) {
            // O parâmetro deve ser amarrado a uma variável através de seu endereço
            // Movemos params[i].valor.v_ptr para %rax, e depois desreferenciamos movendo (%rax) para o registrador alvo
        }
    }

    // Preparar o endereço da função original e realizar a chamada indireta
    // mov f, %rax
    codigo[idx++] = 0x48; 
    codigo[idx++] = 0xb8;
    
    unsigned long long func_addr = (unsigned long long) f;
    memcpy(&codigo[idx], &func_addr, sizeof(unsigned long long));
    idx += sizeof(unsigned long long);

    // call *%rax
    codigo[idx++] = 0xff; 
    codigo[idx++] = 0xd0;

    // Epílogo: desfazer o registro de ativação e retornar
    codigo[idx++] = 0xc9;             // leave
    codigo[idx++] = 0xc3;             // ret
}