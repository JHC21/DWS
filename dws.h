#ifndef dws_h
#define dws_h

#include <time.h>

#define BUFSIZE


/* define Modes */
#define TKMODE 10
#define ALMODE 20
#define SWMODE 30

#define STCURTIME 11

#define STALRTIME 21

#define MSSWTIME 31 // measure stopwatch time
#define MSLPTIME 32 // measure laptime
#define RSSWTIME 33 // reset stopwatch time


#define BUTTONA 'A'
#define BUTTONB 'B'
#define BUTTONC 'C'
#define BUTTOND 'D'



typedef enum boolean {false, true}bool;

bool alarmCheck(time_t currentTime, time_t alarmTime);
void backlightCheck();

char displayTimekeepingMode(char input, char* status);
char displayAlarmMode(char input, char* status);
char displayStopwatchMode(char input, char* status, clock_t measureTime, clock_t lapTime);

void determinePriority(char* inputBuffer);

#endif /* dws_h */
