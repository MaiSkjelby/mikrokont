#include "uart.h"
#include "gpio.h"
#include <stdint.h>

#define UART ((NRF_UART_REG*)0x40002000)

#define RX_PIN 8
#define TX_PIN 6
#define RT_PIN 0xFFFFFFFF // indikerer frakobling (side 541 i datablad)
#define CT_PIN 0xFFFFFFFF

// definerer baudrate til 9600 (bruker side 542)
#define BAUDRATE_DIS 0x00275000

// definerer struct for memory mapping
// bruker volatile nøkkelord for å forhinde at kompilatoren optimaliserer bort tilgang til registeret
// baserer oss på register på side 538 i databladet
// inkluderer ikke deling på 4 i utregningene
typedef struct {
	volatile uint32_t TASKS_STARTRX;    // 0x000   
	volatile uint32_t TASKS_STOPRX;     // 0x004
	volatile uint32_t TASKS_STARTTX;    // 0x008
	volatile uint32_t TASKS_STOPTX;     // 0x00C
    	volatile uint32_t RESERVED0[3];     // 0x01C - 0x00C - 0x004 (plassen til STOPTX) = 28 - 12 - 4 = 12
	volatile uint32_t TASKS_SUSPEND;    // 0x01C
    	volatile uint32_t RESERVED1[56];    // 0x100 - 0x01C - 0x004 (SUSPEND) = 256 - 28 - 4 = 224
    	volatile uint32_t EVENTS_CTS;       // 0x100
    	volatile uint32_t EVENTS_NCTS;      // 0x104
    	volatile uint32_t EVENTS_RXDRDY;    // 0x108
    	volatile uint32_t RESERVED2[4];     // 0x11C - 0x108 - 0x004 = 284 - 264 - 4 = 16
    	volatile uint32_t EVENTS_TXDRDY;    // 0X11c
    	volatile uint32_t RESERVED3[1];     // 0x124 - 0x11C - 0x004 = 292 - 284 - 4 = 4
    	volatile uint32_t EVENTS_ERROR;     // 0x124
    	volatile uint32_t RESERVED4[7];     // 0x144 - 0x124 - 0x004 = 324 - 292 - 4 = 28
    	volatile uint32_t EVENTS_RXTO;      // 0x144
    	volatile uint32_t RESERVED5[46];    // 0x200 - 0x144 - 0x004 = 512 - 324 - 4 = 184
    	volatile uint32_t SHORTS;           // 0x200
    	volatile uint32_t RESERVED6[64];    // 0x304 - 0x200 - 0x004 = 772 - 512 - 4 = 256
    	volatile uint32_t INTENSET;         // 0x304
    	volatile uint32_t INTENCLR;         // 0x308
    	volatile uint32_t RESERVED7[93];    // 0x480 - 0x308 - 0x004 = 1152 - 776 - 4 = 372
    	volatile uint32_t ERRORSRC;         // 0x480
    	volatile uint32_t RESERVED8[31];    // 0x500 - 0x480 - 0x004 = 1280 - 1152 - 4 = 124
    	volatile uint32_t ENABLE;           // 0x500
    	volatile uint32_t RESERVED9[1];     // 0x508 - 0x500 - 0x004 = 1288 - 1280 - 4 = 4
    	volatile uint32_t PSELRTS;          // 0x508
    	volatile uint32_t PSELTXD;          // 0x50C
    	volatile uint32_t PSELCTS;          // 0x510
    	volatile uint32_t PSELRXD;          // 0x514
    	volatile uint32_t RXD;              // 0x518
    	volatile uint32_t TXD;              // 0x51C
    	volatile uint32_t RESERVED10[1];    // 0x524 - 0x51C - 0x004 = 1316 - 1308 - 4 = 4
    	volatile uint32_t BAUDRATE;         // 0x524
    	volatile uint32_t RESERVED11[17];   // 0x56C - 0x524 - 0x004 = 1388 - 1316 - 4 = 68
    	volatile uint32_t CONFIG;           // 0x56C
} NRF_UART_REG;

// initialiseringsfunksjon
void uart_init() {

	// initialiserer GPIO-pinnene som I/O
	GPIO->PIN_CNF[RX_PIN] = (1 << 1) | (3 << 2); // prøver forskjellige måter å sette som pull-up med input
	GPIO->PIN_CNF[TX_PIN] = 1;

	// kobler pins til UART og gjør disconnect kjent
	UART->PSELTXD = TX_PIN;
    	UART->PSELRXD = RX_PIN;
    	UART->PSELRTS = RT_PIN;
    	UART->PSELCTS = CT_PIN;

	// setter baudrate verdi
	UART->BAUDRATE = BAUDRATE_DIS;

	// tydeliggjør ingen paritet
	UART->CONFIG = 0;

	// skrur på UART-modulen
	UART->ENABLE = 4;

	// skrur på melding mottak
	UART->TASKS_STARTRX = 1;
}


// tar imot en enkel bokstav og sender til datamaskinen
void uart_send(char letter) {

	// clearer txdrdy før ny byte legges til
	UART->EVENTS_TXDRDY = 0;
		
	// begynner å sende melding
	UART->TASKS_STARTTX = 1;

	// lagrer bokstaven i TXD
	UART->TXD = letter;

	// venter til sendingen er ferdig
	while (UART->EVENTS_TXDRDY == 0){
	}

	// avslutter TX-tilstand 
	UART->TASKS_STOPTX = 1;	
}

// leser en bokstav fra PCen og returnerer
char uart_read(void) {
	// ser etter data
	if (UART->EVENTS_RXDRDY != 0) {
		// setter til 0 for å unngå pakketap
		UART->EVENTS_RXDRDY = 0;

		//leser mottat data én gang
		return (char)UART->RXD;
	} else {
		// indikerer ingen ny data, lagt til for å unngå blokkering
		return '\0';
	}
}
