#include <msp430.h> 
#include "libs/spi.h"

#define DUMMY_BYTE 0xFF

// COMMANDS =========================================
#define MEASURE_COMMAND 'M'
#define IRRIGATE_COMMAND 'I'
// M -> Enviar medidas
// I -> Irrigar
volatile unsigned char last_command = 0;

// MEASUREMENTS =====================================
volatile unsigned char temperature = 'A';
// 0, 1 ou 2, do mais escuro ao mais claro
volatile unsigned char luminosity_level = 'B';
#define umidity ((P2IN & BIT0) != 0? 1 : 0)
volatile unsigned char measurements_idx = 0;

// IRRIGATE =========================================
volatile unsigned char irrigating = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// Configura o SPI no UCB0
	spiConfigUCB0(0, 0, 1, 0);
	// Habilita a interrupção de recepção no UCB0
	UCB0IE = UCRXIE;

	 __enable_interrupt();

	while(1);

	return 0;
}

volatile unsigned char received_byte;
#pragma vector = USCI_B0_VECTOR
__interrupt void __ucb0_interrupt(void)
{
    switch (__even_in_range(UCB0IV, 0x04)) {
    case USCI_UCRXIFG:
        received_byte = UCB0RXBUF;

        if (received_byte == MEASURE_COMMAND) {
            last_command = MEASURE_COMMAND;
            measurements_idx = 0;
            UCB0TXBUF = DUMMY_BYTE;
            return;
        }

        if (received_byte == IRRIGATE_COMMAND) {
            last_command = IRRIGATE_COMMAND;
            irrigating = 1;
            UCB0TXBUF = DUMMY_BYTE;
            return;
        }

        if (received_byte != DUMMY_BYTE) {
            // Se receber algo que não é um DUMMY nem um
            // comando, enviar um DUMMY e retornar
            UCB0TXBUF = DUMMY_BYTE;
            return;
        }

        // Se receber um dummy, tratar de acordo com o last_command
        if (last_command == MEASURE_COMMAND) {
            switch (measurements_idx)
            {
            case 0:
                UCB0TXBUF = temperature;
                break;
            case 1:
                UCB0TXBUF = luminosity_level;
                break;
            case 2:
                UCB0TXBUF = umidity;
                break;
            default:
                UCB0TXBUF = DUMMY_BYTE;
                break;
            }

            measurements_idx++;
            if (measurements_idx == 3) {
                measurements_idx = 0;
                last_command = 0;
            }

            return;
        }

        if (last_command == IRRIGATE_COMMAND) {
            UCB0TXBUF = irrigating;
            return;
        }

        // Envia um DUMMY para garantir que a comunicação vai continuar
        UCB0TXBUF = DUMMY_BYTE;
        break;
    default:
        break;
    }
}
