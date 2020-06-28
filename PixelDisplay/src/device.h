/*
 * device.h
 *
 * Created: 21.06.2020 13:10:55
 *  Author: Radtke
 */ 


#ifndef DEVICE_H_
#define DEVICE_H_

#include <asf.h>
#include "FatFS/diskio.h"
#include "FatFS/ff.h"

#define SPIMISO				PIO_PA12A_MISO
#define SPIMOSI		     	PIO_PA13A_MOSI
#define SPICLK			    PIO_PA14A_SPCK
#define SPINSS				PIO_PA11A_NPCS0




#endif /* DEVICE_H_ */