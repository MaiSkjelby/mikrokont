#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "gpio.h"

#define BUTTON1_PIN 13
#define BUTTON2_PIN 14
#define LED_PIN_START 17
#define LED_PIN_END 20

// lager en bitmask for led-ene
#define LED_MASK ((1UL << 17) | (1UL << 18) | (1UL << 19) | (1UL << 20))

// initaliserer knappene
void button_init(void) {

    // knappene er aktiv-lav, konfigurers til pull-up
    	GPIO->PIN_CNF[BUTTON1_PIN] = (3 << 2);
    	GPIO->PIN_CNF[BUTTON2_PIN] = (3 << 2);

}

// forsinkelsesfunksjon for debounding
void delay(volatile uint32_t count) {

   	while (count--) {
    }

}

// omdirigerer output fra iprint til UART
// sender hvert tegn individuelt
ssize_t _write(int fd, const void *buf, size_t count) {
	
	// konverterer buffer peker til peker for char 
	// (så den kan tolkes som streng med tegn)
    	char *letter = (char *)buf;
	
	// sender tegn én om gangen
        for (int i = 0; i < count; i++) {
        	uart_send(*letter);
        	letter++;
    	}	

	// standard å returnere count for å være sikker på vellykket sending
    	return count;

}

// tillater bruk av standard input over UARTen
ssize_t _read(int fd, void *buf, size_t count) {

	// tolker bufferpeker som charpeker
    	char *str = (char *)buf;
    	char letter;
    	
	// kaller uart_read frem til gylig tegn er mottatt
    	do {
    	    letter = uart_read();
    	} while(letter == '\0');
    
	// skriver mottat tegn til buffer
    	*str = letter;

    	return 1;

}

int main(void) {
	
	button_init();
    	uart_init();

   
	// konfiguerer led-matrisen
    	for (uint32_t pin = LED_PIN_START; pin <= LED_PIN_END; pin++) {
        	GPIO->PIN_CNF[pin] = 1;
    	}
    	
	// slår led-ene av (aktiv lav)
   	GPIO->OUT |= LED_MASK;
    
    	uint8_t led_on = 0;
    
    	while (1) {

		// OPPGAVE 2.2, 2.3, OG 2.4

		// polling av button 1
        	if (!(GPIO->IN & (1 << BUTTON1_PIN))) {
            		delay(100000);

			// sender 'A' hvis knappen trykkes
            		if (!(GPIO->IN & (1 << BUTTON1_PIN))) {
                		uart_send('A');

				// blokkerer utførelse frem til knappen slippes
                		while (!(GPIO->IN & (1 << BUTTON1_PIN)));
                		delay(100000);
            		}
        	}
        	
		// polling av button 2
        	if (!(GPIO->IN & (1 << BUTTON2_PIN))) {
            		delay(100000);

			// sender 'B' hvis knappen trykkes
            		if (!(GPIO->IN & (1 << BUTTON2_PIN))) {
                		uart_send('B');
               			while (!(GPIO->IN & (1 << BUTTON2_PIN)));
                		delay(100000);
            		}
        	}
        	
		// polling for mottat tegn. leser tegn fra uart
		char received = uart_read();
        	if (received != '\0') {
			// skrur av eller på led-ene når tegn er mottatt
           		if (led_on) {
                		GPIO->OUT |= LED_MASK;
                		led_on = 0;
            		} else {
                		GPIO->OUT &= ~LED_MASK;
                		led_on = 1;
            		}
        	}



		// OPPGAVE 2.5

		/*	
		iprintf("The average grade in TTK%d was in %d was: %c\n\r", 4235, 2022, 'B');
		*/

		// OPPGAVE 2.6
		// kom ikke helt i mål!

	}

    	return 0;

}


