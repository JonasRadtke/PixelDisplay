/*
 * oszillatoren.c
 *
 * Created: 23.05.2017 12:30:01
 *  Author: Radtke
 */ 

#include "display.h"


volatile uint32_t res = 1;
volatile	uint8_t displayout[768] = {170};
	
pixel display[256];

uint8_t gamma8[256];


	
void timerInit (void){
	uint32_t rc;
	// Timer für Oszillatoren
	sysclk_enable_peripheral_clock(ID_TC0); // Clock auf Timer 0
	pio_set_peripheral 	( PIOA, PIO_PERIPH_B, PIO_PA0 );
	
	tc_init(TC, 0,
	TC_CMR_TCCLKS_TIMER_CLOCK1		// Waveform Clock Selection
	| TC_CMR_WAVE					// Waveform mode
	| TC_CMR_WAVSEL_UP_RC			// Bei RC zurücksetzen
	| TC_CMR_ACPA_CLEAR					// RA Effekt
	| TC_CMR_ACPC_SET				// RC Effekt
	);
	
	tc_enable_interrupt(TC, 0, TC_IER_CPCS); // Compare RA Interrupt einschalten
	rc = (SystemCoreClock /2 /US125);	// Samplefrequenz
	tc_write_ra(TC, 0, 1000);
	tc_write_rb(TC, 0, 2500);
	tc_write_rc(TC, 0, rc);				// RC einstellen /Compare oben
	NVIC_EnableIRQ(TC0_IRQn);			// Interrupt einschalten
	tc_start(TC, 0);					// Timer starten
	
	// Timer für
/*	sysclk_enable_peripheral_clock(ID_TC1); // Clock auf Timer 0
	tc_init(TC, 1,
	TC_CMR_TCCLKS_TIMER_CLOCK1		// Waveform Clock Selection
	| TC_CMR_WAVE					// Waveform mode
	| TC_CMR_WAVSEL_UP_RC			// Bei RC zurücksetzen
	);
	tc_enable_interrupt(TC, 1, TC_IER_CPCS); // Compare RC Interrupt einschalten
	rc = (SystemCoreClock /2 /PHASEAKKU_FREQ);	//
	tc_write_rc(TC, 1, rc);				// RC einstellen
	NVIC_EnableIRQ(TC1_IRQn);			// Interrupt einschalten
	tc_start(TC, 1);

	// Timer für NOISE
	sysclk_enable_peripheral_clock(ID_TC2); // Clock auf Timer 0
	tc_init(TC, 2,
	TC_CMR_TCCLKS_TIMER_CLOCK1		// Waveform Clock Selection
	| TC_CMR_WAVE					// Waveform mode
	| TC_CMR_WAVSEL_UP_RC			// Bei RC zurücksetzen
	);
	tc_enable_interrupt(TC, 2, TC_IER_CPCS); // Compare RC Interrupt einschalten
	rc = (SystemCoreClock /2 /NOISEFREQ);	//
	tc_write_rc(TC, 2, rc);				// RC einstellen
	NVIC_EnableIRQ(TC2_IRQn);			// Interrupt einschalten
	tc_start(TC, 2);
	
	return;
	*/
//	sysclk_enable_peripheral_clock(ID_MEM2MEM);
//	NVIC_EnableIRQ(MEM2MEM_IRQn);



}


// DIE TIMER BZW. KLANGERZEUGUNG!
void TC0_Handler()
{


}

// Init des Gamma Korrektur Arrays
void initGammaCorr(uint8_t x[], float gamma, uint32_t bright)
{
	uint32_t i;
	float temp;
	
	for (i=0; i<256; i++) // (i/255)^gamma * 255
	{
		temp = (float)i* ((float)bright * 12) / 120;
		temp = (float)temp/255;
		temp = pow(temp, gamma);
		x[i] = (uint8_t)ceil((temp * 255)); // Gamma Korrektur	
	}	
}


// Copy a BMP in Display Array to Out Buffer Array
void BMPToOutBuffer (pixel x[], uint8_t out[])
{
	uint32_t i; // 0-15
	uint32_t zeile; // 0-15
	uint32_t j = 0;
	
	for (zeile=0; zeile<16; zeile++) // 16 Zeilen
	{
		for (i=0; i<48-1; i+=3)	// 48 byte, 3x16 pro pixel
		{
			if (zeile%2 == 0)	// Gerade Zeilen laufen von links nach rechts, ungerade von rechts nach links
			{
				out[zeile*48+i] = gamma8[x[j].blue];
				out[zeile*48+i+1] =gamma8[x[j].red] ;
				out[zeile*48+i+2]= gamma8[x[j].green] ;
			}
			else
			{
				out[zeile*48+48-3-i] = gamma8[x[j].blue];
				out[zeile*48+48-3-i+1] =gamma8[x[j].red];
				out[zeile*48+48-3-i+2]= gamma8[x[j].green] ;
			}
			
			

			j++;
		}
	}
}

#define ws2812_First "nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
#define ws2812_Second "nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
#define ws2812_Third "nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
#define ws2812_NextByteComp "nop\n\t"

void ws2812_sendarray(uint8_t *data,int datlen)
{
	
	// Systick abschalten
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk  ;
	
	uint32_t maskhi = DISPLAY_OUT;					//ws2812_mask_set;
	uint32_t masklo = DISPLAY_OUT;					//ws2812_mask_clr;
	volatile uint32_t *set = &PIOA->PIO_SODR;	//  Pin Set Register Adresse
	volatile uint32_t *clr = &PIOA->PIO_CODR;	//  Pin Clear Register Adresse
	uint32_t i;
	uint32_t curbyte;

	while (datlen--) {		// Länge einen runter zählen bis null
		curbyte=*data--;	// Nächstes Byte laden

		asm volatile(
		"		lsl %[dat],#24				\n\t"	// 8 bit ganz nach links schieben
		"		movs %[ctr],#8				\n\t"	// Eine 8 laden, für 8 Bit die gesendet werden
		"ilop%=:							\n\t"	// Schleifen
		"		str %[maskhi], [%[set]]		\n\t"	// High Ausgang

		ws2812_First								// Erste 0,4 us von ingesamt 1,25 us warten

		"		lsls %[dat], #1				\n\t"	// Bit eins nach links schieben
		"		bcs one%=					\n\t"	// Wenn es einen Carry gab dann war das Bit eine eins
		"		str %[masklo], [%[clr]]		\n\t"	// Ansonsen Ausgang auf Low
		"one%=:								\n\t"

		ws2812_Second								// Zweite 0,4 us warten
		
		"		str %[masklo], [%[clr]]		\n\t"	// Ausgang auf Low, hier wieder immer auf low geschaltet
		"		subs %[ctr], #1				\n\t"	// von dem 8bit zähler einen abziehen
		"		beq	end%=					\n\t"	// Auf Zero vergleichen, wenn null dann Ende und nächstes Byte

		ws2812_Third								// Dritte 0,4us warten, falls schon zu Ende werden die letzten 0,4 us durch das neu laden der Daten verbraucht
	

		"		b 	ilop%=					\n\t"	// springe absolut zur Schlefie
		"end%=:								\n\t"	// Ende
		:	[ctr] "+r" (i)
		:	[dat] "r" (curbyte), [set] "r" (set), [clr] "r" (clr), [masklo] "r" (masklo), [maskhi] "r" (maskhi)
		);
	}
	
	// Systick einschalten
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */
}