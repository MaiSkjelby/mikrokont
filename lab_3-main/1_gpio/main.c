#include <stdint.h>

#define GPIO ((NRF_GPIO_REGS*)0x50000000)
#define __RESERVED1_SIZE__ 118 // PIN_CNF[0] 0x700 - 0x528 / 4 bytes per ord
#define __BUTTON_1_PIN__   13
#define __BUTTON_2_PIN__   14

typedef struct {
    volatile uint32_t RESERVED0[321];
    volatile uint32_t OUT;
    volatile uint32_t OUTSET;
    volatile uint32_t OUTCLR;
    volatile uint32_t IN;
    volatile uint32_t DIR;
    volatile uint32_t DIRSET;
    volatile uint32_t DIRCLR;
    volatile uint32_t LATCH;
    volatile uint32_t DETECTMODE;
    volatile uint32_t RESERVED1[__RESERVED1_SIZE__];
    volatile uint32_t PIN_CNF[32];
} NRF_GPIO_REGS;

void button_init()
{
    // konfigurerer begge knappene som input med pull-ups (setter PIN_CNF som pull-up)
    GPIO->PIN_CNF[__BUTTON_1_PIN__] = (3 << 2);
    GPIO->PIN_CNF[__BUTTON_2_PIN__] = (3 << 2);
}

int main(){

    for (int i = 17; i <= 20; i++) {
        GPIO->DIRSET = (1 << i);
        GPIO->OUTCLR = (1 << i);
    }

    button_init();

    int sleep = 0;

    while (1) {

        /* sjekker hvis button 1 er trykket
         * (activ lav, så sjekker !(GPIO->IN & (1 << __BUTTON_1_PIN__)))
         * hvis trykket, skru på led-matrisen
         */
        if (!(GPIO->IN & (1 << __BUTTON_1_PIN__))) {
            GPIO->OUTSET |= (1 << 20) | (1 << 19) | (1 << 18) | (1 << 17);       
        }

        /* sjekker hvis button 2 er trykket (aktiv lav)
         * hvis trykket, skru av led-matrisen
         */
        if (!(GPIO->IN & (1 << __BUTTON_2_PIN__))) {
            GPIO->OUTCLR |= (1 << 20) | (1 << 19) | (1 << 18) | (1 << 17);

	}

        sleep = 10000;

        while (--sleep);
    }

    return 0;
}

