#include "dws.h"
#include <time.h>

void increaseValue(char status, struct tm *time){
	switch(status){
		case STSEC:
			time.tm_sec++;
			break;
		case STMIN:
			time.tm_min++;
			break;
		case STHOU:
			time.tm_hour++;
			break;
		case STDAY:
			time.tm_day++;
			break;
		case STMON:
			time.tm_month++;
			break;
		case STYEA:
			time.tm_year++;
			break;
	}

	/* rounding values */	
	if(time.tm_sec > 60){
		time.tm_min++;
		time.tm_sec-=60;
	}
	if(time.tm_min > 60){
		time.tm_hour++;
		time.tm_min-=60;
	}
	if(time.tm_hour > 24){ /* start from 0 */
		time.tm_day++;
		time.tm_hour-=24;
	}
	if(time.tm_day > 30){ // check later
		time.tm_month++;
		time.tm_day-=30;
	}
	if(time.tm_month > 12){
		time.tm_year++;
		time.tm_month-=12;
	}

	return;
}
