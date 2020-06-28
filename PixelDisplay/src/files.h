/*
 * files.h
 *
 * Created: 27.06.2020 09:33:25
 *  Author: Radtke
 */ 


#ifndef FILES_H_
#define FILES_H_

#include "fatfs/ff.h"
#include "structs.h"
#include "display.h"

FRESULT readrootdir(char [][13], uint32_t *);
FRESULT loadNextImage(char dir[][13], pixel x[], uint32_t, uint32_t);
FRESULT loadNextFolder(char dir[][13], uint32_t y , pixel x[]);


#endif /* FILES_H_ */