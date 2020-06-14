/*
 * oszillatoren.h
 *
 * Created: 23.05.2017 12:30:14
 *  Author: Radtke
 */ 

#include "structs.h"

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define US125				(800000)
#define TCHIGH				(1)
#define TCLOWZERO			()
#define	TCLOWONE			()
#define PHASEAKKU_FREQ		(100000)
#define NOISEFREQ			(100000) // At NES 223721
#define ENVELOP_TIME		(1)      // in milliseconds
#define DISPLAY_OUT			(PIO_PA17) 


/*
union dds_cnt_t{
	struct{
		uint8_t byte1;
		uint8_t byte2;
		uint8_t byte3;
		uint8_t phase;
	};
	uint32_t counter;
} ;

typedef struct chan1 {
	// General
	uint8_t oscillator_on;	// Chanel On/off
	uint8_t waveform;		// Waveform, RECTANGLE, TRIANGLE, NOISE
	uint8_t pushed_key;		// Number of pushed Key in this Oscillator
	float frequency;		// Frequency of Note
	
	// Envelope
	uint32_t envelopeVolume;	// Actual Volume
	uint32_t envelopeStep;
	uint32_t envelopeStepAttack;
	
	int32_t adsrCnt;			// Counter for Attack,Delay,Sustain,Release - in milliseconds, counts to zero
	
	int32_t delayTime;		// Length of Delay - in milliseconds
	
	uint32_t sustainVol;	// Sustain Volume (constant)
	int32_t arpegSustainTime; //Sustaintime for Arpeggiator
	
	uint32_t releaseActive;   // Release is Active
	int32_t releaseTime;	// Length of Release - in milliseconds
	
	int32_t burstTime;
	
	int32_t attackTime;
	
	uint8_t arpegNotes[50];
	uint8_t arpegNoteNumber;
	uint8_t arpegPlayCounter;
	uint8_t arpegModeActive;
	uint8_t loadNextNote;
	uint8_t arpegAttackActive;
	uint8_t arpegReleaseActive;
	int32_t sustainTime;
	
	// Waveforms
	uint32_t dutycycle;		    // Duty Cycle in percent 0-100%
	uint32_t rect_low;			// Rectangle, this is the Dutycycle 
	uint32_t rect_end;			// Rectangle, end
	uint32_t rect_count;		// Rectangle counter, for internal counting
	
	uint32_t tri_table_index;	// Indexing the Table of Triangle
	uint32_t tri_stepsize;		 // Stepsize for DDS
	union dds_cnt_t dds_counter;
	
	uint32_t noise_lfsr;
	uint32_t noise_cnt;
	uint32_t noise_divider;
	uint32_t noise_bit;
	uint32_t noise_metal; // Metallic Noise
	
	// Outputs
	uint32_t chan_out;		// Output of Channel
} chan;

typedef struct noiseChannel {
	// General
	uint32_t noise_lfsr;
	uint32_t noise_cnt;
	uint32_t noise_divider;
	uint32_t noise_bit;
	uint32_t noise_metal; // Metallic Noise
} noiseChan;
*/
void timerInit (void);
void ws2812_sendarray(uint8_t *ledarray,int length);
/*void oscillator(chan *);
void noise(chan [], noiseChan *);

void activateChannel(uint8_t key[],Settings set,chan x[], float note[], uint16_t div[]);
int8_t _searchFreeChannel(chan x[], uint8_t key);
void _calculateChannelSettings(chan x[],Settings, uint8_t channelIndex, uint8_t key, float note[], uint16_t div[]);
void envelopChannel(uint8_t key[] ,chan x[], Settings);
void sortInArpegNote(uint8_t*,uint8_t,uint8_t);
void arpeggiator(chan channel[],Settings settings);
*/

#endif /* DISPLAY_H_ */