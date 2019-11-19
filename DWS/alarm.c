#include "dws.h"
#include <stdio.h>


bool alarmCheck(bool *isAlarm, char* input, struct tm *currentTime, struct tm *alarmTime) {
	if (*isAlarm) {
		fprintf(stdout, "\aBeep!\n");
		if (input[0] == BUTTONA || input[0] == BUTTONB || input[0] == BUTTONC || input[0] == BUTTOND) {
			input[0] = 0;
			*isAlarm = false; /* alarm OFF */
			return false;
		}
	}
	else {
		if ((currentTime->tm_hour == alarmTime->tm_hour) && (currentTime->tm_min == alarmTime->tm_min)) {
			*isAlarm = true;
			return true;
		}
	}
	return true;
}
