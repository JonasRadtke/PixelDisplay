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
#define DISPLAY_OUT			(PIO_PA0) 


typedef struct px{
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	}pixel; 

#pragma pack(push, 2)
typedef struct tagBITMAPFILEHEADER {
	uint16_t  bfType;
	uint32_t bfSize;
	uint16_t  bfReserved1;
	uint16_t  bfReserved2;
	uint32_t bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;   // http://msdn.microsoft.com/en-us/library/dd183374%28en-us,VS.85%29.aspx

typedef struct tagBITMAPINFOHEADER {
	uint32_t biSize;
	int32_t  biWidth;
	int32_t  biHeight;
	uint16_t  biPlanes;
	uint16_t  biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t  biXPelsPerMeter;
	int32_t  biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;   // http://msdn.microsoft.com/de-de/library/dd183376%28en-us,VS.85%29.aspx

typedef struct tagRGBTRIPLE {
	uint8_t rgbtBlue;
	uint8_t rgbtGreen;
	uint8_t rgbtRed;
} RGBTRIPLE;                         // http://msdn.microsoft.com/de-de/library/dd162939%28v=VS.85%29.aspx
#pragma pack(pop)

void BMPToOutBuffer (pixel [], uint8_t []);
void initGammaCorr(uint8_t [], float, uint32_t);
void timerInit (void);
void ws2812_sendarray(uint8_t *ledarray,int length);

#endif /* DISPLAY_H_ */