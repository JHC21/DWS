#include "dws.h"
#include <time.h>
typedef struct tm s_tm;

void setTime(s_tm *t) {
	int month_day[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	
	if ((t->tm_year % 4 == 0 && t->tm_year % 100) || t->tm_year % 400 == 0)
		month_day[1] = 29;
	else
		month_day[1] = 28;

	//1900 :0년
	t->tm_year += 1900;
	if (t->tm_year >= 200)
		t->tm_year = 0;

	if (t->tm_mon > 12) 
		t->tm_mon = 1;
	
	if (t->tm_mday > month_day[t->tm_mon - 1])
		t->tm_mday = 1;
	
	if (t->tm_hour >= 24)
		t->tm_hour = 0;

	if (t->tm_min >= 60)
		t->tm_min = 0;

	if (t->tm_sec >= 60)
		t->tm_sec = 0;

}

