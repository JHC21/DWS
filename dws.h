#ifndef dws_h
#define dws_h

#include <time.h>

#define BUFSIZE 4


/* define Modes */
#define TKMODE 10
#define ALMODE 20
#define SWMODE 30

#define STCURTIME 11

#define STSEC 12
#define STMIN 13
#define STHOU 14
#define STDAY 15
#define STMON 16
#define STYEA 17


#define STALRTIME 21

#define MSSWTIME 31 // measure stopwatch time
#define MSLPTIME 32 // measure laptime
#define RSSWTIME 33 // reset stopwatch time


#define BUTTONA 'A'
#define BUTTONB 'B'
#define BUTTONC 'C'
#define BUTTOND 'D'



typedef enum boolean {false, true} bool;


bool alarmCheck(struct tm *currentTime, struct tm *alarmTime);
void backlightCheck();

void determinePriority(char* inputBuffer);

#endif /* dws_h */
