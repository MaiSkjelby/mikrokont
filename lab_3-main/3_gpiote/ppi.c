#include "ppi.h"
#include "gpiote.h"

void ppi_init() {

	// fjerner pending events for sikkerts skyld
	GPIOTE->EVENTS_IN[0] = 0;
   
    	// konfigurerer 4 PPI kanaler for å koble GPIOTE hendelsen til GPIOTE oppgavene
    	// (button 1 skal toggle led matrisen)

	// kanal 1
	PPI->PPI_CH[0].EEP = (uint32_t)&GPIOTE->EVENTS_IN[0];
	PPI->PPI_CH[0].TEP = (uint32_t)&GPIOTE->TASKS_OUT[1];

	// kanal 2
	PPI->PPI_CH[1].EEP = (uint32_t)&GPIOTE->EVENTS_IN[0];
	PPI->PPI_CH[1].TEP = (uint32_t)&GPIOTE->TASKS_OUT[2];

	// kanal 3
	PPI->PPI_CH[2].EEP = (uint32_t)&GPIOTE->EVENTS_IN[0];
	PPI->PPI_CH[2].TEP = (uint32_t)&GPIOTE->TASKS_OUT[3];

	// kanal 4
	PPI->PPI_CH[3].EEP = (uint32_t)&GPIOTE->EVENTS_IN[0];
	PPI->PPI_CH[3].TEP = (uint32_t)&GPIOTE->TASKS_OUT[4];
}
