/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include "main.h"



/*/ Externe Globale Variablen
extern chan channel[8];
extern noiseChan singleNoise;

extern float notes[88];
extern uint32_t triangletab[TRITAB];
extern uint32_t divider[16]; 
*/
volatile uint32_t ticks = 0; // Time since Start (ms)

uint32_t wt = 0;

#define DISPLAY_OUT			(PIO_PA17) 

extern volatile	uint8_t displayout[768];

int main (void)
{
	pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);   // enable external 32.768kHz crystal
	while (!pmc_osc_is_ready_32kxtal());        // wait until oscillator is ready
	sysclk_init();				// 48 Mhz einstellen
	wdt_disable(WDT);			// Watchdog ausschalten
	SystemCoreClockUpdate();	// Systemclock akualisieren
	fpu_enable ();				// Floatingpoint Unit aktivieren, Achtung richtige Optimierung einstellen!
	delay_init(SystemCoreClock);	
	SysTick_Config(SystemCoreClock / 1000);      /* Configure SysTick to generate an interrupt every millisecond */

	pmc_enable_periph_clk(ID_PIOA);	
	pmc_enable_periph_clk(ID_PIOB);
	pio_set_output 	( 	PIOA, PIO_PA0 ,LOW,DISABLE,DISABLE); // Setze Ausgänge
	
	// Initialize Noise Channel (Seed)
	uint32_t i,j;
//	for(i=0; i<8; i++){
//		channel[i].noise_lfsr = 1;
	//}
//	singleNoise.noise_divider = divider[1-1]; // Noise Divider
//	singleNoise.noise_lfsr = 1; // Noise Seed
	
	// Timer initalisieren! 
//	timerInit();

	//TWI I2C Portexpander initalisieren
//	twiInit();

	//UART initalisieren
//	uartInit();
	
	//SPI Init
//	spiInit();
	
	//ADC Init
//	adcInit();


uint32_t wsticks= 0;
uint32_t lauflicht = 0;
uint32_t counter = 0;
	for(j=0; j<768; j++)
	{
		displayout[j] = 0x00;
		displayout[j+1] = 0x00;
		displayout[j+2] = 0x00;
		j++;
		j++;
	}
	uint8_t *ptr = &displayout[0];
	
	while(1)
	{


		if ((ticks) >= lauflicht+25)
		{
			lauflicht = ticks;	// New Timer Value
			
			displayout[counter] = 0x00;
			displayout[counter+1] = 0x00;
			displayout[counter+2] = 0x00;
			
			displayout[counter+3] = 0x00; // Blau
			displayout[counter+4] = 0x04; // Rot
			displayout[counter+5] = 0x00; // Grün
			
			counter += 3;
			
			if ((counter+3) > 768)
			{
				counter = 0;
			}
			
		}


		if ((ticks) >= wsticks+10)
		{
			wsticks = ticks;	// New Timer Value
			
			ptr = &displayout[767];
			ws2812_sendarray(ptr,768);
			ticks += 7;
		}
		else
		{
		__asm ("nop");
		}
	
	}
}



// Systemtakt 1ms
void SysTick_Handler(){
	ticks++;
}

void print_tasten()
{
	uint8_t keys[2];
	char text[200];
	sprintf(text,   " Tasten 1: %d  \n", keys[0]);
	uartsendstring(text);
	sprintf(text,	" Tasten 2: %d  \n\n", keys[1]);
	uartsendstring(text);
	return;
}