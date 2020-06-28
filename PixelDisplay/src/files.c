/*
 * files.c
 *
 * Created: 27.06.2020 08:50:16
 *  Author: Radtke
 */ 

#include "files.h"
#include "device.h"
#include "uartt.h"

char dirNames[100][13];
uint32_t numberOfDirs;

FATFS FatFs;				/* File system object for each logical drive */
FIL File[2];				/* File objects */
DIR dir;					/* Directory object */
FRESULT res;
static FILINFO fno;

FRESULT readrootdir(char x[][13], uint32_t *y)
{
	uint32_t i;
	uint32_t j;
	FRESULT res;
	
	//Init dir Array with 0
	for (i=0; i<100; i++)
	{
		for (j=0; j<12; j++)
		{
			x[i][j] = 0;
		}
	}
	
	// Root Order Öffnen
	res = f_mount(&FatFs, "", 0);
	if (res != FR_OK) return res;
	res = f_opendir(&dir, "");                       /* Open the directory */
	if (res != FR_OK) return res;
	i=0;
	fno.fname[0] = 1;
	// Alle Ordner durchrotieren
	while (fno.fname[0])
	{
		res = f_readdir(&dir, &fno);                   /* Read a directory item */		
		
//		char text[200];	
		if (res == FR_OK)
		{
			if (!(fno.fname[6] == 0x7E) )
			{ // Zu langer Dateiname
				j=0;
//				sprintf(text,	"%s  \r\n", fno.fname);
//				uartsendstring(text);
				while (fno.fname[j])
				{
					x[i][j] = fno.fname[j];
					j++;
				}
				x[i][j] = '\0'; // terminate
				*y = i;
				i++;
			}
		}
	}
	
}


FRESULT loadNextImage(char dir[][13], pixel x[], uint32_t dirIndex, uint32_t bmpIndex)
{
	uint32_t i;
	uint32_t j;
	FRESULT res;
	char path[20];
	
	// New Path for f_open (/**pathname**/**index**.bmp
	path[0] = '/';
	i=1;
	while (dir[dirIndex][i-1] != 0)
	{
		path[i] = dir[dirIndex][i-1];
		i++;
	}
	path[i] = '/';
	i++;
	i += sprintf( &path[i], "%d", bmpIndex );
	i += sprintf( &path[i], ".bmp" );
	path[i] = '\0';
//						char text[200];
//						sprintf(text,	"%s  \r\n", path);
//					uartsendstring(text);
	
	// Datei öffnen
	res = f_open(File, path, FA_READ);
	if (res != FR_OK) return res;
	
	BITMAPFILEHEADER fileheader;				// Variable für FileHeader
	BITMAPFILEHEADER *headptr = &fileheader;	// Pointer darauf

	BITMAPINFOHEADER infoheader;				// Variable für Infoheader
	BITMAPINFOHEADER *infoptr = &infoheader;	// Pointer darauf
	UINT readBytes;
	
	f_read(File, headptr, sizeof(fileheader), &readBytes);		// Fileheader in Variable einlesen
	if (headptr->bfType == 19778)								// Ist es eine BMP?
	{
		res = f_read(File, infoptr, sizeof(infoheader), &readBytes);	// Infoheader in Variable einlesen
		if (res != FR_OK) return res;
	}
	else
	{
		f_close( File );										// Datei schliessen
		return FR_NO_FILE;
	}
	
	uint8_t bmpdata[768];
	uint32_t zeile; // 0-15
	j = 0;

	File->fptr = fileheader.bfOffBits;
	res = f_read(File, &bmpdata[0], 768, &readBytes);	// Infoheader in Variable einlesen
	if (res != FR_OK) return res;


	for (zeile=16; zeile > 0; zeile--)
	{
		for (i=0; i<48-1; i+=3)
		{
			x[j].blue	= bmpdata[(zeile-1)*48+i];
			x[j].green	= bmpdata[(zeile-1)*48+i+1];
			x[j].red	= bmpdata[(zeile-1)*48+i+2];
			j++;
		}
	}
	res = f_close( File );
	if (res != FR_OK) return res;
	return FR_OK;
	
	
}

FRESULT loadNextFolder(char dir[][13], uint32_t y , pixel x[])
{
	static uint32_t dirIndex = 0;
	static uint32_t bmpIndex = 0;
	FRESULT res;
	
	//Random MUSS NOCH EINGEBUNDEN WERDEN!

	
	res = loadNextImage(dir, x, dirIndex, bmpIndex);
	if (res != FR_OK) 
	{
		dirIndex++; 
		if (dirIndex > y) dirIndex = 0; // durchrotieren falls Ordner zu hoch
		bmpIndex = 0; // Falls es nicht klappt mit nächsten Ordner fortfahren
	}
	else
	{
		bmpIndex++;	// Ansonsten nächstes Bild
	}	
}