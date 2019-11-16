#include "dws.h"
#include <stdio.h>

bool alarmCheck(bool isAlarm,struct tm *currentTime, struct tm *alarmTime){
    if(isAlarm){
       fprintf(stdout, "\aBeep!\n" );
        if (kbhit() != 0) {
            isAlarm = false; /* alarm OFF */
            return false;
        }
    }
    if(currentTime==alarmTime){
        isAlarm = true;

        return true;
    }
    return false;
}
