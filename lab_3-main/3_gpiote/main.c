#include <stdbool.h>
#include <stdint.h>
#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"


int main(void) {

    gpiote_init();

    ppi_init();

    // aktiverer PPI kanalene
    PPI->CHENSET = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);

    // uendelig loop for å holde CPU-en opptatt
    // viser at PPI og GPIOTE utfører oppgaven uten CPU
    while (true) {
    }
    
}
