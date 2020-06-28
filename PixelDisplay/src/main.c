/**
 * \file
 *
 * \brief Empty user application template
 *
 */

#include "main.h"

// Externe Globale Variablen

extern uint8_t gamma8[256];
extern	uint8_t displayout[768];
extern	pixel display[256];
extern UINT Timer1;
extern uint32_t numberOfDirs;


extern char dirNames[100][13];


volatile uint32_t ticks = 0; // Time since Start (ms)


uint32_t wt = 0;


FATFS FatFs;				/* File system object for each logical drive */
FIL File[2];				/* File objects */
DIR dir;					/* Directory object */
FRESULT res;
static FILINFO fno;
int main (void)
{
	pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);   // enable external 32.768kHz crystal
	while (!pmc_osc_is_ready_32kxtal());        // wait until oscillator is ready
	sysclk_init();				// 48 Mhz einstellen
	wdt_disable(WDT);			// Watchdog ausschalten
	SystemCoreClockUpdate();	// Systemclock akualisieren
	fpu_enable ();				// Floatingpoint Unit aktivieren, Achtung richtige Optimierung einstellen!
//	delay_init(SystemCoreClock);	
	SysTick_Config(SystemCoreClock / 1000);      /* Configure SysTick to generate an interrupt every millisecond */

	pmc_enable_periph_clk(ID_PIOA);	
	pmc_enable_periph_clk(ID_PIOB);
	pio_set_output 	( 	PIOA, PIO_PA0 ,LOW,DISABLE,DISABLE); // Setze Ausgänge
	

	uint32_t i,j;


	
	// Timer initalisieren! 
//	timerInit();

	//TWI I2C Portexpander initalisieren
//	twiInit();

	//UART initalisieren
	uartInit();
	
	//SPI Init
//	spiInit();
	
	//ADC Init
//	adcInit();


	uint32_t wsticks= 0; // Timer for Display Refresh
	uint32_t bmpToOutTicks = 0; // Timer for copy to Outbuffer

	uint8_t *ptr = &displayout[0];
	
	initGammaCorr(gamma8, 2.2, 5); // Gamma inkl. Helligkeit initalisieren 
	
	// Read All Directory Names on SD Card
	res = readrootdir(dirNames, &numberOfDirs);

	// Mail loop
	while(1)
	{

		// Refresh Screen and read next Frame
		if ((ticks) >= bmpToOutTicks+100)
		{
			bmpToOutTicks = ticks;	// New Timer Value
			loadNextFolder(dirNames, numberOfDirs, display);	// Next Image from Folder or next Folder
			BMPToOutBuffer (display, displayout);				// Copy Image to Screenbuffer					
	
			ws2812_sendarray(&displayout[767],768);				// Refresh Screen
			ticks += 7; // Systemtimer was deactivated fpr 7 ms
		}
	
	}
}



// Systemtakt 1ms
void SysTick_Handler(){
	ticks++;
	disk_timerproc();	/* Disk timer process */
}

void print_tasten()
{
	uint8_t keys[2];
	char text[200];
//	sprintf(text,   " Tasten 1: %d  \n", keys[0]);
	uartsendstring(text);
//	sprintf(text,	" Tasten 2: %d  \n\n", keys[1]);
	uartsendstring(text);
	return;
}