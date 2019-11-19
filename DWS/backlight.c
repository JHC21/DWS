//
//  backlight.c
//  DWS
//
//  Created by minjyo on 12/11/2019.
//  Copyright © 2019 minjyo. All rights reserved.
//
#include "dws.h"
#include <stdio.h>


void backlightCheck(bool isOff){
    if(isOff){
        printf("\033[0m");
    } else {
        printf("\033[1;33m");
    }
    return;
}
