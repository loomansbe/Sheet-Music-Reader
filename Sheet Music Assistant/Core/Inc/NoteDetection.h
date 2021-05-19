/*
 * NoteDetection.h
 *
 *  Created on: May 18, 2021
 *      Author: bnloo
 */

#ifndef INC_NOTEDETECTION_H_
#define INC_NOTEDETECTION_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f413h_discovery_lcd.h"
#include <stdbool.h>
#include "fatfs_storage.h"

typedef struct LineClass {
  int startPixelRow, endPixelRow;
} LineClass;

typedef struct NoteClass {
  int startPixelRow, startPixelColumn;
  char noteName;
} NoteClass;

bool checkNoteSpace(int i, int j, uint8_t * imageArray);

NoteClass * findNotes(NoteClass * noteArray, uint8_t * imageArray);

void findLines(LineClass *lineArray, uint8_t * imageArray);

char lineToNoteType(int i);

char spaceToNote(int i);

char checkBetweenLines(LineClass * lineArray, NoteClass * noteArray);

NoteClass * determineNoteTypes(LineClass * lineArray, NoteClass * noteArray);

LineClass * findFlags(uint8_t * imageArray);

void outputNoteLetters(uint8_t * arr);


#ifdef __cplusplus
}
#endif
#endif /* INC_NOTEDETECTION_H_ */
