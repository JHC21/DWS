//
//  alarmCheck.c
//  DWS
//
//  Created by minjyo on 12/11/2019.
//  Copyright © 2019 minjyo. All rights reserved.
//

#include "dws.h"

bool alarmCheck(bool isAlarm,struct tm *currentTime, struct tm *alarmTime){
    if(isAlarm){
       // soundBeep();
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
}  //지호 화이팅~^^~
