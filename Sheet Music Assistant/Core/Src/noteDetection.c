#include <stdio.h>
#include "noteDetection.h"
#include <stdbool.h> 
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <inttypes.h>
extern int LCD_PIXEL_HEIGHT = 240;
extern int LCD_PIXEL_WIDTH = 240;
extern int RGB_BYTE_PER_PIXEL = 2;
extern int NOTE_PIXEL_SIZE = 15;
extern int STAFF_COUNT = 5;
extern int MAX_NOTE_COUNT = 6;
extern int NOTE_DIAMETER = 45;
extern int DISTANCE_FROM_LINE = 5;

extern int E_LINE = 0;
extern int G_LINE = 1;
extern int B_LINE = 2;
extern int D_LINE = 3;
extern int F_LINE = 4;

extern int F_SPACE = 0;
extern int A_SPACE = 1;
extern int C_SPACE = 2;
extern int E_SPACE = 3;


bool checkNoteSpace(int i, int j, uint8_t * imageArray)
{
   bool completeSquare = true;
   for(int k = i; k < (i + NOTE_PIXEL_SIZE); k = k + 1) //One navigates rows
   {
      for(int l = j; l < j + (NOTE_PIXEL_SIZE * RGB_BYTE_PER_PIXEL); l = l + RGB_BYTE_PER_PIXEL) //One navigates pixels/columns
      {
         if (k < LCD_PIXEL_HEIGHT || l < LCD_PIXEL_WIDTH * RGB_BYTE_PER_PIXEL) //Check that k and l are in bounds of the image borders
         {
            uint16_t *rgb565 = (uint16_t*)imageArray + ((LCD_PIXEL_HEIGHT*k) + (l/2)); //Get current pixel color
            if (*rgb565 != 0)
            {
               completeSquare = false; //If the current pixel in the square is not black, then the entire area isn't black
            }
         }
         else
         {
            completeSquare = false;
         }
      }
   }
   return completeSquare;
}


NoteClass * findNotes(NoteClass * noteArray, uint8_t * imageArray)
{
   int noteCount = 0;
   int distance = 0;
   bool existingNote = false;
   int yVal = 0;

   for (int i = 0; i < (LCD_PIXEL_WIDTH * RGB_BYTE_PER_PIXEL); i = i + RGB_BYTE_PER_PIXEL) //Move pixels
   {
      for (int j = 0; j < LCD_PIXEL_HEIGHT; j = j + 1) //Move rows
      { //I changed the order of the inner and outer loops 
        //Because notes will never be placed vertically under each other
         uint16_t *rgb565 = (uint16_t*)imageArray + ((LCD_PIXEL_HEIGHT*j) + (i/2)) - 1;
         if (*rgb565 == 0)
         {
            bool completeSquare = true;
            completeSquare = checkNoteSpace(j, i, imageArray); //Swapped the input order due to loop change
            if (completeSquare)
            {
               int startingPixelRow = j;
               int startingPixelColumn = i/RGB_BYTE_PER_PIXEL; //The array indexes are 240x480 due to 2 bytes per pixel
                                                               //The horizontal index moves twice per loop and is double the 240 index
               i = i + (NOTE_PIXEL_SIZE * RGB_BYTE_PER_PIXEL);

               for(int z = 0; z < noteCount; z++ )
               {
                  distance = (int)sqrt((double)((startingPixelRow-noteArray[z].startPixelRow)*(startingPixelRow-noteArray[z].startPixelRow))+(double)((startingPixelColumn-noteArray[z].startPixelColumn)*(startingPixelColumn-noteArray[z].startPixelColumn)));
                  if(distance < NOTE_DIAMETER)
                     existingNote = true;
               }

               if (noteCount < MAX_NOTE_COUNT && !existingNote)
               {
                  startingPixelRow += 7;
                  startingPixelColumn += 19;
                  struct NoteClass newNote = {startingPixelRow, startingPixelColumn};
                  noteArray[noteCount] = newNote;
                  noteCount = noteCount + 1; //Creating note structs and storing them in an array like the line structs
                  /* printf("%i", startingPixelRow);
                  printf("%s", "-");
                  printf("%i", startingPixelColumn);
                  printf("%s", "  ");  */
                  //yVal = 240 - startingPixelRow;
                  /*printf("%i", startingPixelColumn);
                  printf("%s", "-");
                  printf("%i", startingPixelRow);
                  printf("%s", "  ");*/
               }
               existingNote = false;
            }
         }
      }
   }
}


void findLines(LineClass *lineArray, uint8_t * imageArray)
{
   int pixelCounter;

   int lineCounter = 0;

   bool completeLine = true;
   int startingRow = 0;
   int endingRow = 0;


   for (int i = 0; i < LCD_PIXEL_HEIGHT; i = i + 1)
   {
      if (!completeLine)
      { //Check if at least one full row of pixels were the expected color 
         if (startingRow != endingRow + 1)
         {
            struct LineClass newLine = {startingRow, endingRow};
            if (lineCounter < STAFF_COUNT)
            {
               lineArray[lineCounter] = newLine;
               lineCounter = lineCounter + 1;
            }
         }
         startingRow = i;
         endingRow = i;
      }
      completeLine = true;
      pixelCounter = 0;
      for (int j = 0; j < (LCD_PIXEL_WIDTH); j = j + 1) //Move over number of bytes per pixel
      {
         uint16_t *rgb565 = (uint16_t*)imageArray + ((LCD_PIXEL_HEIGHT*i) + j);
         
         if (*rgb565 == 0)
         {
            pixelCounter = pixelCounter + 1;
         }
      }
      if (pixelCounter < LCD_PIXEL_WIDTH/RGB_BYTE_PER_PIXEL) //Check to see if a threshold of black pixels per line is met
      {
         completeLine = false;
         endingRow = i - 1;
      }
   }
}

char lineToNoteType(int i)
{
   if (i == E_LINE)
   {
      return 'E';
   }
   else if (i == G_LINE)
   {
      return 'G';
   }
   else if (i == B_LINE)
   {
      return 'B';
   }
   else if (i == D_LINE)
   {
      return 'D';
   }
   else if (i == F_LINE)
   {
      return 'F';
   }
}

char spaceToNote(int i)
{
   if (i == F_SPACE)
   {
      return 'F';
   }
   else if (i == A_SPACE)
   {
      return 'A';
   }
   else if (i == C_SPACE)
   {
      return 'C';
   }
   else if (i == E_SPACE)
   {
      return 'E';
   }
}

char checkBetweenLines(LineClass * lineArray, NoteClass * noteArray)
{  
   for (int i = 0; i < MAX_NOTE_COUNT; i = i + 1)
   {
      int currentNoteRow = noteArray[i].startPixelRow;
      for (int j = 0; j < STAFF_COUNT - 1; j = j + 1)
      {
         if((currentNoteRow < lineArray[j+1].startPixelRow - DISTANCE_FROM_LINE) && (currentNoteRow > lineArray[j].endPixelRow + DISTANCE_FROM_LINE))
         {
            if (noteArray[i].noteName == NULL)
            {
               noteArray[i].noteName = spaceToNote(j);
            }
         }
      }
   }
}

NoteClass * determineNoteTypes(LineClass * lineArray, NoteClass * noteArray)
{
   for(int i = 0; i < STAFF_COUNT; i = i + 1)
   {
      for(int j = 0; j < MAX_NOTE_COUNT; j = j + 1)
      {
         int currentNoteRow = noteArray[j].startPixelRow;
         if (currentNoteRow < (lineArray[i].endPixelRow + DISTANCE_FROM_LINE) && currentNoteRow > (lineArray[i].startPixelRow - DISTANCE_FROM_LINE))
         {
            noteArray[j].noteName = lineToNoteType(i);
         }
      }
   }

   checkBetweenLines(lineArray, noteArray);
}

// Finds the vertical lines on the notes
LineClass * findFlags(uint8_t * imageArray)
{
   printf("Finding Flags\n");
   //Start with the first verticl column of pixles
   //bool completeLine = false;
   int flagCounter = 0;
   int flagWidth = 3;
   int startingCol = 0;
   int endingCol = 0;
   int length = 0;
   int width = 0;
   int startColArray [240] = {};
   //int endArray [20] = {};
   
/*
   int startColArray [240] = {};
   // Check each column if a pixel is black
   for (int i = 0; i < (LCD_PIXEL_WIDTH * RGB_BYTE_PER_PIXEL); i = i + RGB_BYTE_PER_PIXEL)
   {
       for (int j = 0; j < LCD_PIXEL_HEIGHT; j++)
       {
            uint16_t *rgb565 = (uint16_t*)imageArray + (240*j)+ i;
            //printf("%i ", *rgb565);
            if (*rgb565 == 0)
            {
                flagCounter++;
                startingCol = j;
            }
            if (*rgb565 != 0)
            {
                endingCol = j;
                if (flagCounter >= 80)
                {
                    printf("%i %i %i\n", flagCounter, startingCol, endingCol);
                    flagCounter = 0;
                }
            }
       }
       
   }
   */
    while(length < LCD_PIXEL_HEIGHT)
    {
        uint16_t *rgb565 = (uint16_t*)imageArray + (240*length) + width;
        if (*rgb565 == 0)
        {
            
            width += RGB_BYTE_PER_PIXEL;
            flagCounter++;
        }
        else if (*rgb565 != 0)
        {
            //printf("W %i ", flagCounter);
            if (flagCounter != flagWidth)
            {
                width += RGB_BYTE_PER_PIXEL;
                //flagCounter = 0;
            }
            else if (flagWidth == flagCounter)
            {
                startColArray[startingCol] = width;
                startingCol++;
                width += RGB_BYTE_PER_PIXEL;
            }
        }
        length++;
        width = 0;
    }
    printf("Flags: %i\n", flagCounter);
    int size = sizeof(startColArray)/sizeof(int);
    for (int i = 0; i < size; i++)
    {
       // printf("S %i ", startColArray[i]);
        int j = i+1;
        for (j = i+1; j < size; j++)
        {
            if (startColArray[i] == startColArray[j])
                break;
        }
        if (i == j)
        {
            endingCol++;
            printf("E %i ", endingCol);
        }
    }
    
}


void outputNoteLetters(uint8_t * arr) {

   LineClass lineArray [STAFF_COUNT]; //Stores structs containing starting and ending line rows
   NoteClass noteArray [MAX_NOTE_COUNT];

   findLines(lineArray, arr);
   
   /*for(int i = 0; i < STAFF_COUNT; i = i + 1) //Demonstrates accessing array of line objects/structs
   {
         int startingRow = lineArray[i].startPixelRow;
         int endingRow = lineArray[i].endPixelRow;
         printf("%i", startingRow);
         printf("%s", "-");
         printf("%i", endingRow);
         printf("%s", "  ");
   }
   putchar('\n');*/
   
   findNotes(noteArray, arr);
   //putchar('\n');

   determineNoteTypes(lineArray, noteArray);

   for(int i = 0; i < sizeof(noteArray)/sizeof(noteArray[0]); i = i + 1)
   {
		char currentNoteType = noteArray[i].noteName;
		switch (currentNoteType) {
			case 'A' :
				BSP_LCD_DisplayStringAt(noteArray[i].startPixelColumn, 240 - noteArray[i].startPixelRow, (uint8_t*)"A", LEFT_MODE);
				break;
			case 'B' :
				BSP_LCD_DisplayStringAt(noteArray[i].startPixelColumn, 240 - noteArray[i].startPixelRow, (uint8_t*)"B", LEFT_MODE);
				break;
			case 'C' :
				BSP_LCD_DisplayStringAt(noteArray[i].startPixelColumn, 240 - noteArray[i].startPixelRow, (uint8_t*)"C", LEFT_MODE);
				break;
			case 'D' :
				BSP_LCD_DisplayStringAt(noteArray[i].startPixelColumn, 240 - noteArray[i].startPixelRow, (uint8_t*)"D", LEFT_MODE);
				break;
			case 'E' :
				BSP_LCD_DisplayStringAt(noteArray[i].startPixelColumn, 240 - noteArray[i].startPixelRow, (uint8_t*)"E", LEFT_MODE);
				break;
			case 'F' :
				BSP_LCD_DisplayStringAt(noteArray[i].startPixelColumn, 240 - noteArray[i].startPixelRow, (uint8_t*)"F", LEFT_MODE);
				break;
			case 'G' :
				BSP_LCD_DisplayStringAt(noteArray[i].startPixelColumn, 240 - noteArray[i].startPixelRow, (uint8_t*)"G", LEFT_MODE);
				break;
			default :
				break;
      }
      /*printf("%c", currentNoteType);
      putchar('\n');*/
   }
}
