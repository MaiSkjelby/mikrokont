#include "gpiote.h"
#include "gpio.h"


// slet med feil pindefinisjoner noen steder, så definerer alle for å få mer oversikt
// fra PIN_CNF[0], side 136

#define GPIO_PIN_CNF_DIR_Pos 0
#define GPIO_PIN_CNF_DIR_Input 0

#define GPIO_PIN_CNF_INPUT_Pos 1
#define GPIO_PIN_CNF_INPUT_Connect 0

#define GPIO_PIN_CNF_PULL_Pos 2
#define GPIO_PIN_CNF_PULL_Pullup 3

// for å skru av SENSE (GPIOTE skal handtere dette)
#define GPIO_PIN_CNF_SENSE_Pos 16
#define GPIO_PIN_CNF_SENSE_Disabled 0

#define PIN_BUTTON1 13
#define PIN_LED1    17
#define PIN_LED2    18
#define PIN_LED3    19
#define PIN_LED4    20

void gpiote_init() {
	
	// fjerner potensielle gamle events
	GPIOTE->EVENTS_IN[0] = 0;

	// konfigurer button 1 som input med pull-up
    	GPIO->PIN_CNF[PIN_BUTTON1] = 	(GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | // setter som inngang
                                 	(GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | // kobler input buffer
                                 	(GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) | // aktiverer pull-up
                                 	(GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos); // skrur av sense så GPIOTE kan handtere det
  
	// konfiguerer kanal 0 på GPIOTE til button 1 hendelse
	GPIOTE->CONFIG[0] = 	(1 << 0) | // setter mode til event
				(PIN_BUTTON1 << 8) | // setter PSEL posisjon til button 1 pin
                        	(2 << 16); // setter polaritet slik at button 1 drar lavt


   	// konfigurerer gpiote kanalene 1 - 4 for led oppgaver (beynner høy pga aktiv lav, skrur på toggle, 
    	// setter som output)

    	GPIO->DIRSET = (1 << PIN_LED1) | (1 << PIN_LED2) | (1 << PIN_LED3) | (1 << PIN_LED4);
    	GPIO->OUTSET = (1 << PIN_LED1) | (1 << PIN_LED2) | (1 << PIN_LED3) | (1 << PIN_LED4);

    	GPIOTE->CONFIG[1] = (3 << 0) |
                    		(PIN_LED1 << 8) |
                    		(3 << 16) |
                    		(1 << 20);
	GPIOTE->CONFIG[2] = (3 << 0) |
                    		(PIN_LED2 << 8) |
                   		(3 << 16) |
                    		(1 << 20);
	GPIOTE->CONFIG[3] = (3 << 0) |
                    		(PIN_LED3 << 8) |
                    		(3 << 16) |
                    		(1 << 20);
	GPIOTE->CONFIG[4] = (3 << 0) |
                    		(PIN_LED4 << 8) |
                    		(3 << 16) |
                    		(1 << 20);

    // må ikke aktivere GPIOTE-kanalene selv
}
