#include <stdio.h>
#include <string.h>

// Definições do protocolo
#define MSG_OK "DADOS_VALIDOS"
#define ACK "ACK"
#define NACK "NACK"

// Buffer de comunicação
char canal_tx[50];
char canal_rx[50];
int erro_transmissao = 0;

// Protothread Transmissora (máquina de estados)
void transmissora() {
    static int estado = 0;
    static int tentativas = 0;

    switch (estado) {
        case 0: // Envio inicial
            printf("\n[TX] Enviando dados...\n");
            strcpy(canal_tx, MSG_OK);

            if (erro_transmissao) {
                strcpy(canal_tx, "ERRO");
            }

            estado = 1; // vai esperar resposta
            break;

        case 1: // Esperando resposta
            if (strcmp(canal_rx, ACK) == 0) {
                printf("[TX] Recebido ACK. Transmissão concluída!\n");
                estado = 2; // finaliza
            } else if (strcmp(canal_rx, NACK) == 0) {
                printf("[TX] Recebido NACK. Reenviando...\n");
                tentativas++;
                if (tentativas > 3) {
                    printf("[TX] Número máximo de tentativas excedido!\n");
                    estado = 2; 
                } else {
                    estado = 0; // tenta de novo
                }
            }
            break;

        case 2:
            // Encerrado
            break;
    }
}

// Protothread Receptora (máquina de estados)
void receptora() {
    static int estado = 0;

    switch (estado) {
        case 0: // Aguardando mensagem
            if (strcmp(canal_tx, MSG_OK) == 0) {
                printf("[RX] Dados recebidos corretamente.\n");
                strcpy(canal_rx, ACK);
            } else if (strlen(canal_tx) > 0) {
                printf("[RX] Erro nos dados recebidos!\n");
                strcpy(canal_rx, NACK);
            }
            break;
    }
}

// Função principal
int main(void) {
    printf("=== Simulação de protocolo com máquinas de estados (protothreads fake) ===\n");

    // Ativa erro inicial para simular retransmissão
    erro_transmissao = 1;

    for (int i = 0; i < 10; i++) {
        transmissora();
        receptora();

        // Após algumas tentativas, remove o erro
        if (i == 2) {
            erro_transmissao = 0;
        }
    }

    return 0;
}
